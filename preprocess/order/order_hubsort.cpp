#include "order_hubsort.h"
#include "../utils/adjlist.h"
#include "../utils/tools.h"
#include "../utils/heap.h"
#include "../utils/pvector.h"
#include<omp.h>
#include<limits.h>
#include <algorithm>
#include "../utils/sliding_queue.h"
#include <parallel/basic_iterator.h>
#include <iostream>
#include <vector>
#include <parallel/algorithm>
using namespace std;


typedef std::pair<ul, ul> degree_nodeid_t;

vector<ul> order_hubsort(const Adjlist &g) 
{
    //开始时间
     vector<ul> rank(g.n, UINT_MAX);
    auto numVertices = g.n;
    auto numEdges    = g.e;
    
    pvector<degree_nodeid_t> degree_id_pairs(numVertices);


    ul avgDegree  = numEdges / numVertices;
    //cout << "avgDegree == "<<avgDegree <<endl;
    ul hubCount {0};//hub的数量,比平均度大，就是一个hub

   /* STEP 1 - 遍历所有顶点的度，生成degree_id_pairs，和hub的数量 */
#pragma omp parallel for reduction(+ : hubCount)
    for (ul v = 0; v < numVertices; ++v) 
    {
        degree_id_pairs[v] = std::make_pair(g.get_degree(v), v);
        if (g.get_degree(v) > avgDegree) 
        {
            ++hubCount;
        } 
    }

    /* Step 2 - 对度数进行并行排序,前面n个是hub*/
    __gnu_parallel::sort(degree_id_pairs.begin(), degree_id_pairs.end(),std::greater<degree_nodeid_t>());

    /* Step 3 - 对hub进行遍历，重新排序1-n；基于排序的degree list做remap，只对hubs[Only for hubs] */
#pragma omp parallel for
    for (ul n = 0; n < hubCount; ++n) 
    {
        rank[degree_id_pairs[n].second] = n;
    }
    //clearing space from degree pairs
    pvector<degree_nodeid_t>().swap(degree_id_pairs);

    /* Step 4 - 分配一个remap给非hub顶点assigning a remap for (easy) non hub vertices */
    auto numHubs = hubCount;
    SlidingQueue<ul> queue(numHubs);

//启动1个并行区域
#pragma omp parallel
    {
        //每个线程创建1个本地队列缓冲区
        //SlidingQueue<uintE> queue(numHubs);local_size=numHubs / omp_get_max_threads()
        QueueBuffer<ul> lqueue(queue, numHubs / omp_get_max_threads());
#pragma omp for
        for (ul n = numHubs; n < numVertices; ++n) 
        {
            //没有被映射，保存原id
            if (rank[n] == UINT_MAX) 
            {
                // This steps preserves the ordering of the original graph (as much as possible)
                rank[n] = n;
            }
            else //已经被映射，是一个hub，将[numHubs,numVertices]中hub被映射的id和hub交换一下
            {
                ul remappedTo = rank[n];//2
                if (rank[remappedTo] == UINT_MAX) 
                {
                    // safe to swap Ids because the original vertex is a non-hub 
                    rank[remappedTo] = n;//交换一下
                }
                else 
                {
                    // Cannot swap ids because original vertex was a hub (swapping 
                    // would disturb sorted ordering of hubs - not allowed)
                    //在[numHubs,numVertices]之间的hub保存到lqueue中
                    lqueue.push_back(n);
                }
            }
        }
        //每个线程调用flush方法，以附加由于“中心”约束而无法重新映射的任何剩余顶点。
        //flush() 方法确保在继续图形处理流程的下一步之前，所有线程都完成了其重新映射。
        lqueue.flush(); 
    }
    //the queue keeps a list of vertices 
    //where a simple swap of locations is not possible
    queue.slide_window(); 
    


    /* Step 5 - assigning remaps for remaining non hubs */
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
   
    //cout<<"HubSort Map Time"<<endl;
    return rank;
}
