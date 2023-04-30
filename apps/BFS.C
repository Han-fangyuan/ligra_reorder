#include "ligra.h"

//BFS_F————————————————————————————
struct BFS_F 
{
	//成员，表示每个定点的父节点【在BFS算法中，每个顶点在搜索过程中都会被标记为已访问，并记录其父节点的编号，以便最后生成BFS树。】
	uintE* Parents;

	//构造函数
	BFS_F(uintE* _Parents) : Parents(_Parents) {}

	//更新顶点的父节点信息，非原子操作：s和d代表源点和目标顶点
	//如果目标顶点d的父节点还未被标记，那么就将其父节点设置为源顶点s，并返回true，否则返回false。
	inline bool update (uintE s, uintE d) 
	{
		if(Parents[d] == UINT_E_MAX) { Parents[d] = s; return 1; }
		else return 0;
	}

	//更新顶点的父节点信息，原子操作
	inline bool updateAtomic (uintE s, uintE d)
	{ 
		return (CAS(&Parents[d],UINT_E_MAX,s));
	}

	//cond function checks if vertex has been visited yet
	inline bool cond (uintE d) 
	{ 
		return (Parents[d] == UINT_E_MAX); 
	} 
};


template <class vertex>    
void Compute(graph<vertex>& GA, commandLine P) 
{
	//cout<<"进入compute函数"<<endl;
	//查看是否给出源点，没有给，默认为0;long r = atol(argv[i+1])
	long start = P.getOptionLongValue("-r",1);

	long n = GA.n;

	//初始化大小为n的Patrents数组，初始化UINT_E_MAX
	//creates Parents array, initialized to all -1, except for start
	uintE* Parents = newA(uintE,n);
	parallel_for(long i=0;i<n;i++) Parents[i] = UINT_E_MAX;

	//起始顶点
	Parents[start] = start;
	//创建大小为n的顶点子集，只包含起始顶点
	vertexSubset Frontier(n,start); 
	//cout<<Frontier.isDense<<endl;
	//进入循环，直到frontier为空
	//cout<<"进入循环"<<endl;
	//cout<<"Frontier.isEmpty()=="<<Frontier.isEmpty()<<endl;
	while(!Frontier.isEmpty())
	{
		//在循环中，使用edgeMap函数将当前frontier所有顶点的邻居顶点添加到顶点自己output
		//使用BFS_F函数对象更新Parents数组中的元素
		vertexSubset output = edgeMap(GA, Frontier, BFS_F(Parents));   

		Frontier.del();//顶点子集删除把s或d删除
		Frontier = output; //set new frontier
	} 
	Frontier.del();
	free(Parents); 
}
