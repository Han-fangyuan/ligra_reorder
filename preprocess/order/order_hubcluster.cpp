#include "order_hubcluster.h"
#include "../utils/sliding_queue.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"
#include "../utils/tools.h"
#include<omp.h>
#include<limits.h>
#include <vector>
#include <algorithm>


using namespace std;


vector<ul> order_hubcluster(const Adjlist &g) 
{
 
    vector<ul> deg(g.n, 0);
    auto numVertices = g.n;
    auto numEdges    = g.e;
    vector<ul> rank(g.n, UINT_MAX);

    
    ul avgDegree  = numEdges / numVertices;
    ul hubCount {0};

    const int PADDING = 64 / sizeof(ul);
    ul* localOffsets = new ul[omp_get_max_threads() * PADDING]();
    ul partitionSz = numVertices / omp_get_max_threads();

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        ul startID = partitionSz * tid;
        ul stopID  = partitionSz * (tid + 1);
        if (tid == omp_get_max_threads() - 1) 
        {
            stopID = numVertices;    
        }
        //
        for (ul n = startID; n < stopID; ++n) 
        {
        
            if (g.get_degree(n) > avgDegree) 
            {
                ++localOffsets[tid * PADDING];
                rank[n] = 1;
            }
                
        }
    }
    
    ul sum {0};
    
    for (int tid = 0; tid < omp_get_max_threads(); ++tid) 
    {
        auto origCount = localOffsets[tid * PADDING];
        localOffsets[tid * PADDING] = sum;
        sum += origCount;
    }

    /* Step II - 先给hubremap，hubassign a remap for the hub vertices first */
#pragma omp parallel
    {
        ul localCtr {0};
        int tid = omp_get_thread_num();
        ul startID = partitionSz * tid;
        ul stopID  = partitionSz * (tid + 1);
        if (tid == omp_get_max_threads() - 1) 
        {
            stopID = numVertices;
        }
        for (ul n = startID; n < stopID; ++n) 
        {
            if (rank[n] != UINT_MAX) 
            {
                rank[n] = localOffsets[tid * PADDING] + localCtr;
                ++localCtr;
            }
        }
    }
    delete[] localOffsets;

    /* 3 - 赋值[numhubs,n)，assigning a remap for (easy) non hub vertices */
    auto numHubs = sum;
    SlidingQueue<ul> queue(numHubs);
#pragma omp parallel
    {
        
        QueueBuffer<ul> lqueue(queue, numHubs / omp_get_max_threads());
#pragma omp for
        for (ul n = numHubs; n < numVertices; ++n) 
        {
            if (rank[n] == UINT_MAX) 
            {
                rank[n] = n;
            }
            else 
            {
                ul remappedTo = rank[n];
                if (rank[remappedTo] == UINT_MAX) 	 rank[remappedTo] = n;
                else 	lqueue.push_back(n);
            }
        }
        lqueue.flush(); 
    }
    queue.slide_window(); //the queue keeps a list of vertices where a simple swap of locations is not possible

    /* 4 - 赋值[0,numhubs)，assigning remaps for remaining non hubs */
    ul unassignedCtr {0};
    auto q_iter = queue.begin();
#pragma omp parallel for
    for (ul n = 0; n < numHubs; ++n) 
    {
        if (rank[n] == UINT_MAX) 
        {
            ul u = *(q_iter + __sync_fetch_and_add(&unassignedCtr, 1));
            rank[n] = u;
        }
    }
    
    //cout << "HubCluster Map Time:"<<endl;
    return rank;
     
}
