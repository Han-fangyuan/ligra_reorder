#include "order_deg.h"
#include "../utils/adjlist.h"
#include "../utils/edgelist.h"
#include "../utils/tools.h"
#include <algorithm>

using namespace std;

inline bool compare_nodedeg_desc (Keyvalue &a, Keyvalue &b) 
{ // sort degree DESC, node ASC
	return ( a.val > b.val or (a.val == b.val and a.key < b.key) );
}
inline bool compare_nodedeg_asc (Keyvalue &a, Keyvalue &b) { // sort degree ASC, node ASC
	return ( a.val < b.val or (a.val == b.val and a.key < b.key) );
}



vector<ul> rank_from_deg(const vector<ul> &deg, const ul &n, bool desc);

//if(order_name == "deg" ) rank = order_deg(h,true);主程序
vector<ul> order_deg(Edgelist &g, bool desc) 
{
 
	Debug("Order deg from edgelist")
	g.compute_degrees();
	return rank_from_deg(g.deg, g.n, desc);

}


vector<ul> rank_from_deg(const vector<ul> &deg, const ul &n, bool desc) 
{
  
	vector<Keyvalue> toSort; 
	toSort.reserve(n);
  	for (ul u = 0; u < n; ++u)
    	toSort.push_back( Keyvalue(u, deg[u]) );

	//desc==true
	if(desc) sort(toSort.begin(), toSort.end(), compare_nodedeg_desc);
	else     sort(toSort.begin(), toSort.end(), compare_nodedeg_asc);

	vector<ul> rank; 
	rank.reserve(n);
	for (ul u = 0; u < n; ++u)
		rank[ toSort[u].key ] = u;


	return rank;
}

/*
vector<ul> order_degOut(Edgelist &g, bool desc) 
{
	Debug("Order degOut from edgelist")
  g.compute_degrees();
  return rank_from_deg(g.degOut, g.n, desc);
}

vector<ul> order_degIn(Edgelist &g, bool desc) 
{
	Debug("Order degIn from edgelist")
  g.compute_degrees();
  return rank_from_deg(g.degIn, g.n, desc);
}
*/
