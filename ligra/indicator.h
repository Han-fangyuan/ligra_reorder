#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <algorithm>
#include <cassert>
#include<cmath>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/numeric/conversion/cast.hpp>

using namespace boost::multiprecision;



//#include "../ligra/index_map.h"
//#include "../ligra/edgeMap_utils.h"
using namespace std;

string add(const string& s1, const string& s2) 
{
    string res;
    int carry = 0, i = s1.size() - 1, j = s2.size() - 1;
    while (i >= 0 || j >= 0) {
        int sum = carry;
        if (i >= 0) sum += s1[i--] - '0';
        if (j >= 0) sum += s2[j--] - '0';
        res += to_string(sum % 10);
        carry = sum / 10;
    }
    if (carry) res += to_string(carry);
    reverse(res.begin(), res.end());
    return res;
}

string divide(const string& num, int divisor) 
{
    string res;
    int remainder = 0;
    for (char c : num) {
        int digit = c - '0';
        remainder = remainder * 10 + digit;
        res += to_string(remainder / divisor);
        remainder %= divisor;
    }
    while (res.size() > 1 && res[0] == '0') res.erase(0, 1);
    return res;
}

uintT absolute(uintT a,uintT b )
{
	if(a>=b) return a-b;
	else return b-a;
}


template <class vertex>
void computePackingFactor(graph<vertex> GA) 
{
    size_t elemSz=sizeof(double);
    auto numVertices = GA.n;
    auto numEdges    = GA.m;
    vertex *origG    = GA.V;
    uintT avgDegree  = numEdges / numVertices;
    
    size_t cacheBlkSz {64};
    size_t vDataSz        = numVertices * elemSz; //Total size of vData array in Bytes
    size_t numCacheBlocks = (vDataSz + (cacheBlkSz-1)) / cacheBlkSz; //number of cache blocks to completely store entire vData
    size_t vtxPerBlk {0}; 
    size_t hubCacheBlocks {0};
    size_t numHubs {0};
    double hotSetSize_before {0};
    double hotSetSize_after  {0}; 
    double packingFactor     {0}; 
    
    if (elemSz < cacheBlkSz) {
        vtxPerBlk = cacheBlkSz / elemSz;
        #pragma omp parallel for reduction (+ : hubCacheBlocks, numHubs)
        for (uintE b = 0; b < numCacheBlocks; ++b) {
            bool hasHubs {false};
            for (uintE v = b * vtxPerBlk; v < (b+1) * vtxPerBlk; ++v) {
                if (origG[v].getOutDegree() > avgDegree) {
                    hasHubs = true;
                    ++numHubs;
                }
            }
            if (hasHubs) {
                ++hubCacheBlocks;
            }
        }
        hotSetSize_before = hubCacheBlocks * 64;
        hotSetSize_after  = ((numHubs + (vtxPerBlk-1)) / (vtxPerBlk)) * 64; 
        hotSetSize_after  = (((numHubs*elemSz) + (cacheBlkSz-1)) / (cacheBlkSz)) * 64; 
        packingFactor     = static_cast<double>(hotSetSize_before) / static_cast<double>(hotSetSize_after);
    }
    
    //std::cout << "Number of hubs = " << numHubs << std::endl;
    //std::cout << "HotSet size in MB (before reordering) = " << static_cast<double>(hotSetSize_before) / (1024 * 1024) << std::endl;
    //std::cout << "HotSet size in MB (after  reordering) = " << static_cast<double>(hotSetSize_after) / (1024 * 1024) << std::endl;
    std::cout << "Packing Factor : " << packingFactor << std::endl;
   
}


//AID[n]：每个顶点都有一个AID
template <class vertex>
void compute_AID(graph<vertex> GA)
{
    
	auto numVertices = GA.n;
    auto numEdges    = GA.m;
    vertex *v    = GA.V;
	long long vertex_total_out = 0;
    long long graph_total_out = 0;
    long long vertex_total_in = 0;
    long long graph_total_in = 0;
    vector<uintT> AID_out(numVertices);
    vector<uintT> AID_in(numVertices);

    //计算AID出度
	for(uintT i=0;i<numVertices;i++)
	{
		for(uintE j = 0;j < v[i].getOutDegree();j++)
		{
			auto temp =absolute(v[i].getOutNeighbor(j),i);
			vertex_total_out += temp;
		}
        if(v[i].getOutDegree()==0)  AID_out[i]=0;
		else AID_out[i]=vertex_total_out/v[i].getOutDegree();
        graph_total_out += AID_out[i];
        vertex_total_out=0;

	}
	auto average_AID_out = graph_total_out / numVertices;

    //计算AID入度
    for(uintT i=0;i<numVertices;i++)
	{
		for(uintE j = 0;j < v[i].getInDegree();j++)
		{
			auto temp =absolute(v[i].getInNeighbor(j),i);
			vertex_total_in += temp;
		}
        if(v[i].getInDegree()==0)  AID_in[i]=0;
		else AID_in[i]=vertex_total_in/v[i].getInDegree();
        graph_total_in += AID_in[i];
        vertex_total_in=0;

	}
    auto average_AID_in = graph_total_in / numVertices;

	cout << "average_AID_out : " << average_AID_out << endl;
    cout << "average_AID_in : " << average_AID_in << endl;

}

//图指标
template <class vertex>
void compute_gap(graph<vertex> GA)
{
	auto numVertices = GA.n;
    auto numEdges    = GA.m;
	long long total = 0;
    vertex *v    = GA.V;
	vector<uintT> bandwidth_out(numVertices);
    
	for(uintT i=0;i<numVertices;i++)
	{
		uintT max_gap=0;
		for(uintE j = 0;j < v[i].getOutDegree();j++)
		{
			auto temp =absolute(v[i].getOutNeighbor(j),i);
			total += temp;
			if(temp > max_gap)	max_gap = temp;
		}
		bandwidth_out[i]=max_gap;
	}
	auto gap_bandwidth = max_element(bandwidth_out.begin(), bandwidth_out.end());
    long long gap_profile = total / numEdges;
	auto gap_average_bandwidth_out = accumulate(bandwidth_out.begin(), bandwidth_out.end(), 0.0) / numVertices;
    total = 0;
    vector<uintT> bandwidth_in(numVertices);
	for(uintT i=0;i<numVertices;i++)
	{
		uintT max_gap=0;
		for(uintE j = 0;j < v[i].getInDegree();j++)
		{
			auto temp =absolute(v[i].getInNeighbor(j),i);
			total += temp;
			if(temp > max_gap)	max_gap = temp;
		}
		bandwidth_in[i]=max_gap;
	}
	auto gap_average_bandwidth_in = accumulate(bandwidth_in.begin(), bandwidth_in.end(), 0.0) / numVertices;
	
    cout << "gap_profile : " << gap_profile << endl;
    cout << "gap_bandwidth : " << *gap_bandwidth << endl;
	cout << "gap_average_bandwidth_out : " << gap_average_bandwidth_out << endl;
	cout << "gap_average_bandwidth_in : " << gap_average_bandwidth_in << endl;
    
	
}


template <class vertex>
void indicator(graph<vertex> G) 
{
	computePackingFactor(G);
	compute_AID(G);//这个指标计算还有问题
	compute_gap(G);
	
}
