
#define WEIGHTED 1
#include "ligra.h"

//1.BF_F结构体
struct BF_F 
{
  intE* ShortestPathLen;
  int* Visited;
  BF_F(intE* _ShortestPathLen, int* _Visited) : ShortestPathLen(_ShortestPathLen), Visited(_Visited) {}
  //Update ShortestPathLen if found a shorter path
  inline bool update (uintE s, uintE d, intE edgeLen) 
  { 
    intE newDist = ShortestPathLen[s] + edgeLen;
    if(ShortestPathLen[d] > newDist) {
      ShortestPathLen[d] = newDist;
      if(Visited[d] == 0) { Visited[d] = 1 ; return 1;}
    }
    return 0;
  }
  //atomic Update
  inline bool updateAtomic (uintE s, uintE d, intE edgeLen){ 
    intE newDist = ShortestPathLen[s] + edgeLen;
    return (writeMin(&ShortestPathLen[d],newDist) &&
	    CAS(&Visited[d],0,1));
  }
  inline bool cond (uintE d) { return cond_true(d); }
};

//2.BF_Vertex_F——————————————————————————————
//reset visited vertices
struct BF_Vertex_F 
{
  int* Visited;
  BF_Vertex_F(int* _Visited) : Visited(_Visited) {}
  inline bool operator() (uintE i){
    Visited[i] = 0;
    return 1;
  }
};

//Compute函数——————————————————————————————————
template <class vertex>
void Compute(graph<vertex>& GA, commandLine P) 
{
	long start = P.getOptionLongValue("-r",0);
	long n = GA.n;
	//initialize ShortestPathLen to "infinity"
	intE* ShortestPathLen = newA(intE,n);
	{parallel_for(long i=0;i<n;i++) ShortestPathLen[i] = INT_MAX/2;}
	ShortestPathLen[start] = 0;

	int* Visited = newA(int,n);
	{parallel_for(long i=0;i<n;i++) Visited[i] = 0;}

	vertexSubset Frontier(n,start); //initial frontier

	long round = 0;
	while(!Frontier.isEmpty()){
	if(round == n) {
		//negative weight cycle
		{parallel_for(long i=0;i<n;i++) ShortestPathLen[i] = -(INT_E_MAX/2);}
		break;
	}
	vertexSubset output = edgeMap(GA, Frontier, BF_F(ShortestPathLen,Visited), GA.m/20, dense_forward);
	vertexMap(output,BF_Vertex_F(Visited));
	Frontier.del();
	Frontier = output;
	round++;
	}
	Frontier.del(); 
	free(Visited);
	free(ShortestPathLen);
}