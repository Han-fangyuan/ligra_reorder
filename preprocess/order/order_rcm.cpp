// See header for general documentation

#include <algorithm>
#include "order_rcm.h"
#include "../utils/tools.h"
#include "../utils/edgelist.h"
#include "../utils/adjlist.h"
#include "order_deg.h"
#include "../utils/bfs.h"


using namespace std;

//else if(order_name == "rcm") rank = order_rcm(h,directed);
vector<ul> order_rcm(Edgelist &h, bool directed) 
{

  vector<ul> rank_deg = order_deg(h, false); // degree ASC,升序排列
  vector<ul> rank_bfs;

  if(directed) 
  {
    Dadjlist g(h, rank_deg);
    rank_bfs = rank_from_order(algo_bfs(g)); // Cuthill–McKee
  }
  else 
  {
    Uadjlist g(h, rank_deg);
    rank_bfs = rank_from_order(algo_bfs(g)); // Cuthill–McKee
  }

  //最终结果
  vector<ul> rank; 
  rank.reserve(h.n);
  for (ul u = 0; u < h.n; u++) 
  {
    rank[u] = h.n-1 - rank_bfs[rank_deg[u]]; // compose reverse Cuthill–McKee with degree
  }
  return rank;
}
