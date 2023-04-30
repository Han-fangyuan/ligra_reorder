
#include "ligra.h"

//atomically do bitwise-OR of *a with b and store in location a
template <class ET>
inline void writeOr(ET *a, ET b) 
{
	volatile ET newV, oldV; 
	do {oldV = *a; newV = oldV | b;}
	while ((oldV != newV) && !CAS(a, oldV, newV));
}

struct Radii_F 
{
	intE round;
	intE* radii;
	long* Visited, *NextVisited;

	//构造函数
	Radii_F(long* _Visited, long* _NextVisited, intE* _radii, intE _round) : 
	Visited(_Visited), NextVisited(_NextVisited), radii(_radii), round(_round) {}

	//Update function does a bitwise-or
	inline bool update (uintE s, uintE d)
	{ 
		long toWrite = Visited[d] | Visited[s];
		if(Visited[d] != toWrite)
		{
			NextVisited[d] |= toWrite;
			if(radii[d] != round) { radii[d] = round; return 1; }
		}
		return 0;
	}

	//atomic Update
	inline bool updateAtomic (uintE s, uintE d)
	{ 
		long toWrite = Visited[d] | Visited[s];
		if(Visited[d] != toWrite)
		{
			writeOr(&NextVisited[d],toWrite);
			intE oldRadii = radii[d];
			if(radii[d] != round) return CAS(&radii[d],oldRadii,round);
		}
		return 0;
	}

	inline bool cond (uintE d) { return cond_true(d); }
};

//function passed to vertex map to sync NextVisited and Visited
struct Radii_Vertex_F 
{
	long* Visited, *NextVisited;
	Radii_Vertex_F(long* _Visited, long* _NextVisited) : Visited(_Visited), NextVisited(_NextVisited) {}
	inline bool operator() (uintE i) 
	{
		Visited[i] = NextVisited[i];
		return 1;
	}
};

//compute函数——————————————————————————
//基于采样的图半径估计算法，思想：随机选择一些起始节点进行遍历，在每轮遍历中更新节点的半径估计值。
//时间复杂度主要取决于采样的起始节点数量，因此在保证精度的前提下，采样的节点数越少，算法的时间复杂度越低
template <class vertex>
void Compute(graph<vertex>& GA, commandLine P) 
{
	long n = GA.n;
	intE* radii = newA(intE,n);
	long* Visited = newA(long,n), *NextVisited = newA(long,n);

	//所有节点的半径估计值为-1，
	{parallel_for(long i=0;i<n;i++) {
		radii[i] = -1;
		Visited[i] = NextVisited[i] = 0;
	}}

	//随机选择初始节点，半径估计值设为0
	long sampleSize = min(n,(long)64);
	uintE* starts = newA(uintE,sampleSize);

	//initial set of vertices
	{parallel_for(ulong i=0;i<sampleSize;i++) 
	{ 
		uintE v = hashInt(i) % n;
		radii[v] = 0;
		starts[i] = v;
		NextVisited[v] = (long) 1<<i;
	}}

	//
	vertexSubset Frontier(n,sampleSize,starts); //initial frontier of size 64

	intE round = 0;
	while(!Frontier.isEmpty())
	{
		round++;
		vertexMap(Frontier, Radii_Vertex_F(Visited,NextVisited));
		//对当前frontier进行edgeMap操作，将frontier中每个节点的邻居节点加入下一轮遍历的frontier中
		//同时更新邻居节点的半径估计值为当前节点半径估计值加1
		vertexSubset output = edgeMap(GA,Frontier,Radii_F(Visited,NextVisited,radii,round));
		Frontier.del();
		Frontier = output;
	}

	free(Visited); 
	free(NextVisited); 
	Frontier.del(); 
	free(radii); 
}