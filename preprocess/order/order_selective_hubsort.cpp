#include <iostream>

#include "../utils/adjlist.h"
#include "../utils/heap.h"
#include "../utils/pvector.h"
#include "order_hubsort.h"
#include<omp.h>
#include<limits.h>
#include <algorithm>

#include <parallel/basic_iterator.h>
#include <iostream>
#include <vector>
#include <parallel/algorithm>
using namespace std;

const double THRESHOLD {4.0f};




/* 
  Determine if the structure of the graph is amenable to benefit from 
  lightweight reordering techniques. 

  The implementation is a simple scan of the entire vertex space
  to find the cache lines that contain atleast one hub

  NOTE: we found that reordering is most effective for pull-based apps. 
  Hence, the following function assumes out-degree sorting by default
*/


bool computePackingFactor(const Adjlist &g) 
{
    size_t elemSz=sizeof(double);
    auto numVertices = g.n;
    auto numEdges    = g.e;
    //vertex *origG    = GA.V;
    ul avgDegree  = numEdges / numVertices;
    
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
        for (ul b = 0; b < numCacheBlocks; ++b) {
            bool hasHubs {false};
            for (ul v = b * vtxPerBlk; v < (b+1) * vtxPerBlk; ++v) 
            {
                if (g.get_degree(v) > avgDegree) {
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
    //std::cout << "Packing Factor = " << packingFactor << std::endl;
    bool result = packingFactor > THRESHOLD;
    return result;
}


vector<ul> order_selective_hubsort(const Adjlist &g) 
{
    //开始时间

    vector<ul> rank;
    rank.reserve(g.n);

    if(computePackingFactor(g))
    {
        rank = order_hubsort(g);
    }
    else
    {
        for(ul i=0;i<g.n;i++)
        {
            rank[i]=i;
        }
    }
    
    //cout<<"selective_HubSort Map Time"<<endl;
    return rank;
}