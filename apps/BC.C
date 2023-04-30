//中介中心性

#include "ligra.h"
#include <vector>

typedef double fType;

//1.BC_F结构体——————————————————————
//函数:BC_F(NumPaths,Visited)
//实现了算法的前向遍历过程，即对每个顶点的路径数进行更新，并标记该顶点已被访问过
struct BC_F 
{
	fType* NumPaths;
	bool* Visited;
	//构造函数
	BC_F(fType* _NumPaths, bool* _Visited) : NumPaths(_NumPaths), Visited(_Visited) {}

	//成员函数1：对每个顶点进行更新，前向遍历过程
	//该顶点路径数+来源顶点的路径数。如果路径之前为0，返回true
	inline bool update(uintE s, uintE d)
	{ 
		fType oldV = NumPaths[d];
		NumPaths[d] += NumPaths[s];
		return oldV == 0.0;
	}
	//成员函数2：atomic Update
	//使用原子操作更新，保证并发更新的正确性
	inline bool updateAtomic (uintE s, uintE d) 
	{ 
		volatile fType oldV, newV; 
		do { 
			oldV = NumPaths[d]; newV = oldV + NumPaths[s];
		} while(!CAS(&NumPaths[d],oldV,newV));
		return oldV == 0.0;
	}
	//成员函数3：cond
	//检查一个顶点是否被访问过。如果该顶点未被访问，则返回true，否则返回false。
	inline bool cond (uintE d) { return Visited[d] == 0; } //check if visited
};

//2.BC_Back_F结构体——————————————————————
//
struct BC_Back_F 
{
	fType* Dependencies;
	bool* Visited;
	//构造函数
	BC_Back_F(fType* _Dependencies, bool* _Visited) : Dependencies(_Dependencies), Visited(_Visited) {}

	//成员函数1：Update function for backwards phase
	//在反向传播阶段中被调用，用于更新节点的依赖值。
	//它将目标节点的依赖值加上源节点的依赖值，并返回目标节点的原始依赖值是否为 0.0。
	inline bool update(uintE s, uintE d){ 
	fType oldV = Dependencies[d];
	Dependencies[d] += Dependencies[s];
	return oldV == 0.0;
	}
	//成员函数2：atomic Update
	//
	inline bool updateAtomic (uintE s, uintE d) 
	{ 
		volatile fType oldV, newV;
		do {
			oldV = Dependencies[d];
			newV = oldV + Dependencies[s];
		} while(!CAS(&Dependencies[d],oldV,newV));
		return oldV == 0.0;
	}
	//成员函数3：
	//函数用于检查节点是否被访问过。如果节点已被访问过，则返回 true；否则返回 false。
	inline bool cond (uintE d) { return Visited[d] == 0; } //check if visited
};

//3.BC_Vertex_F 
//vertex map function to mark visited vertexSubset
//结构体的作用：在前向遍历和后向遍历过程中，用于标记图中的节点是否已被访问过
struct BC_Vertex_F 
{
	bool* Visited;
	BC_Vertex_F(bool* _Visited) : Visited(_Visited) {}
	//函数调用运算符重载函数，因此这个结构体实际上是一个函数对象。
	//这个函数对象接受一个uintE 类型的参数i，表示需要访问的节点编号。
	//在函数体内，它将该节点标记为已访问（即 Visited[i] = 1），然后返回1，表示该节点已被访问。
	//因为这个函数对象需要作为参数传递给vertexMap函数，所以返回值必须是一个布尔类型
	inline bool operator() (uintE i) 
	{
		Visited[i] = 1;
		return 1;
	}
};

//4.BC_Back_Vertex_F
//vertex map function (used on backwards phase) to mark visited vertexSubset
//and add to Dependencies score
struct BC_Back_Vertex_F 
{

	bool* Visited;
	fType* Dependencies, *inverseNumPaths;//节点的依赖值，节点的逆路径数量
	//构造函数
	BC_Back_Vertex_F(bool* _Visited, fType* _Dependencies, fType* _inverseNumPaths) : 
	Visited(_Visited), Dependencies(_Dependencies), inverseNumPaths(_inverseNumPaths) {}

	//重载括号运算符
	inline bool operator() (uintE i) 
	{
		Visited[i] = 1;
		Dependencies[i] += inverseNumPaths[i];
		return 1; 
	}
};


//compute函数————————————————————————————————————————————————
//并行中介中心性算法实现，遍历图来计算节点的中介中心性
template <class vertex>
void Compute(graph<vertex>& GA, commandLine P) 
{

	long start = P.getOptionLongValue("-r",0);
	//cout << "start == " << start <<endl;
	long n = GA.n;

	//表示从起始节点到每个节点的路径数。
	//对于每个节点i，NumPaths[i]被初始化为0，除了起始节点，它的NumPaths值被初始化为1
	fType* NumPaths = newA(fType,n);
	{parallel_for(long i=0;i<n;i++) NumPaths[i] = 0.0;}
	NumPaths[start] = 1.0;

	//每个节点是否被访问过，初始节点true
	bool* Visited = newA(bool,n);
	{parallel_for(long i=0;i<n;i++) Visited[i] = 0;}
	Visited[start] = 1;

	//Frontier：当前遍历的节点集合，初始只包括起始节点
	vertexSubset Frontier(n,start);

	//levels：保存每轮遍历时的节点集合
	vector<vertexSubset> Levels;
	Levels.push_back(Frontier);

	//得出一个levels数组
	long round = 0;
	while(!Frontier.isEmpty())//frontier不为空，执行循环
	{
		round++;
		vertexSubset output = edgeMap(GA, Frontier, BC_F(NumPaths,Visited));
		vertexMap(output, BC_Vertex_F(Visited)); //mark visited
		Levels.push_back(output); //save frontier onto Levels
		Frontier = output;
	}

	//依赖数组
	fType* Dependencies = newA(fType,n);
	{parallel_for(long i=0;i<n;i++) Dependencies[i] = 0.0;}

	//取倒数invert numpaths
	fType* inverseNumPaths = NumPaths;
	{parallel_for(long i=0;i<n;i++) inverseNumPaths[i] = 1/inverseNumPaths[i];}

	//
	Levels[round].del();

	//reuse Visited
	{parallel_for(long i=0;i<n;i++) Visited[i] = 0;}
	Frontier = Levels[round-1];
	vertexMap(Frontier,BC_Back_Vertex_F(Visited,Dependencies,inverseNumPaths));

	//转置图
	GA.transpose();
	for(long r=round-2;r>=0;r--) 
	{ 
		
		edgeMap(GA, Frontier, BC_Back_F(Dependencies,Visited), -1, no_output);
		Frontier.del();
		Frontier = Levels[r]; //gets frontier from Levels array
		vertexMap(Frontier,BC_Back_Vertex_F(Visited,Dependencies,inverseNumPaths));
	}

	Frontier.del();

	//Update dependencies scores
	parallel_for(long i=0;i<n;i++) 
	{
		Dependencies[i]=(Dependencies[i]-inverseNumPaths[i])/inverseNumPaths[i];
	}

	free(inverseNumPaths);
	free(Visited);
	free(Dependencies);
}