#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include<omp.h>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include "utils/CLI11.h" // options parser
#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/adjlist.h"
#include "utils/edgelist.h"
#include "order/order_rand.h"
#include "order/order_deg.h"
#include "order/order_rcm.h"
#include "order/order_gorder.h"
#include "order/order_ldg.h"
#include "order/order_minla.h"
#include "order/order_hubcluster.h"
#include "order/order_hubsort.h"
#include "order/order_dbg.h"
#include "order/order_selective_hubsort.h"
#include "order/order_slashburn.h"


using namespace std;


int main(int argc, char** argv) 
{

	
  	CLI::App app{"读取a list of edges并根据各种顺序排列其节点"};//没用

  	string filename, order_name="original", algo_name="no", output_file="ord.auto.txt";
	bool directed; ul number = 0;
	
	app.add_option("file", filename, "Text file: list of `a b` edges with nodes IDs ranging from 0 to N-1")->required();
	app.add_option("order", order_name, "Order used to relabel the nodes")->required()->check(CLI::IsMember({
		  			"original", "rcm","ldg","deg","hubsort","hubcluster","selective_hubsort",
					"gorder","slashburn","minla","minloga"}, CLI::ignore_case));
	
	app.add_flag("-d,!-u,--directed,!--undirected", directed,"Specify if the graph is directed or undirected; multiple edges are not accepted");
	app.add_option("-o,--output", output_file, "File in which to output the order")->capture_default_str();
	app.add_option("-n", number, "Numeric parameter used in different orders")->capture_default_str();//gorder里的

	CLI11_PARSE(app, argc, argv);

	ifstream file(filename);
	
	ul n = 0;
	vector<ul> rank;

	cout<<"directed=="<<directed<<endl;
	
	// 不使用重排序算法
	if(order_name == "original" or order_name == "rand") 
	{
		n = get_number_vertices(file);
    	rank = order_identity(n);

		if(order_name == "rand") 
		{
			Debug("Order rand from filestream")
			srand ( unsigned ( time(0) ) );
    		random_shuffle( rank.begin(), rank.end() );
		}
	}
	//重排序算法
	else 
	{
		//edgelist格式的文件进行排序
		Edgelist h(file); 
		n = h.n;
    	rank.reserve(n);

		//deg,rcm,gorder
		if(order_name == "deg" ) rank = order_deg(h,true);
		else if(order_name == "rcm") rank = order_rcm(h,directed);
		else if(order_name == "gorder") 
		{
		  number = (number == 0) ? 5 : number;
		  rank = complete_gorder(h, number,directed);
		}
		//minla,minloga,slashburn,ldg，hubsort，hubcluster，selective hubsort
		else 
		{
			Adjlist* g;
			if(directed) g = new Dadjlist(h);
			else g = new Uadjlist(h);
			if(order_name == "ldg")  rank = order_ldg(*g);
			else if(order_name == "hubsort")  rank = order_hubsort(*g);
			else if(order_name == "selective_hubsort")  rank = order_selective_hubsort(*g);
			else if(order_name == "hubcluster")  rank = order_hubcluster(*g);
			else if(order_name == "dbg")  rank = order_dbg(*g);
      		else if(order_name == "minla")  rank = order_minla(*g);
      		else if(order_name == "minloga")  rank = order_minloga(*g);
      		else if(order_name == "slashburn")  rank = order_slashburn(*g);
			else { Alert("Unknown order `" << order_name <<"`"); return 1; }
      
   	    }
	}
	
	
	c_printorder(rank, n, output_file.c_str());
	

	return 0;
}
