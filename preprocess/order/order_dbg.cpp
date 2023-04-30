#include "../utils/adjlist.h"
#include "../utils/heap.h"
#include "../utils/tools.h"
#include<omp.h>
#include<limits.h>
#include <algorithm>

#include <vector>
#include <parallel/algorithm>
#include<assert.h>
using namespace std;


//rank = order_deg(h, /*desc=*/true);
vector<ul> order_dbg(const Adjlist &g) 
{
    //计算时间
   
    vector<ul> rank(g.n, UINT_MAX);
    ul num_edges = g.e;
    ul num_vertices = g.n;
    
    ul avg_vertex = num_edges / num_vertices;
    
    const ul& av = avg_vertex;
    cout <<av<<endl;
    
    ul bucket_threshold[] = {av/2, av, av*2, av*4, av*8, av*16, av*32, av*64, av*128, av*256, av*512, static_cast<ul>(-1)};
    int num_buckets = 8;//设置了8个分桶
    if ( num_buckets > 11 ) 
    {
        // if you really want to increase the bucket count, add more thresholds to the bucket_threshold above.
            std::cout << "Unsupported bucket size: " << num_buckets << std::endl;
            assert(0);
    }
    //输出桶的threshold最后1个桶threshold==无穷大
    bucket_threshold[num_buckets-1] = static_cast<ul>(-1);
    for(int i=0;i< num_buckets;i++){
        cout<<"bucket_threshold["<<i<<"]=="<<bucket_threshold[i]<<endl;
    }
    
    //8个桶顶点数组
    vector<ul> bucket_vertices[num_buckets];
    const int num_threads = omp_get_max_threads();

    //
    vector<ul> local_buckets[num_threads][num_buckets];

    //本地桶是由每个OpenMP线程维护的一个向量列表，可以在不同线程之间共享。
    //“schedule(static)”指定了在迭代过程中如何分配迭代次数给不同的线程。在这里，“static”表示每个线程将获得一个连续的迭代块，这些块的大小相等。
    // This loop relies on a static scheduling,并行调度

    #pragma omp parallel for schedule(static)
    for ( ul i = 0 ; i < num_vertices ; i++ ) 
    {
        for ( unsigned int j = 0 ; j < num_buckets ; j++ ) 
        {
            const ul & count = g.get_degree(i);
            if ( count <= bucket_threshold[j] ) 
            {
                local_buckets[omp_get_thread_num()][j].push_back(i);
                break;
            }
        }
    }
     
    //这段代码中，定义了一个整型变量temp_k：用于记录当前迭代过程中已经处理的本地桶中的元素个数。
    //同时，也定义了一个二维数组start_k，用于存储每个本地桶在全局排序数组中的起始索引。
    //其中，第一维表示线程编号，第二维表示桶编号。这个数组的初始化是通过嵌套两个for循环实现的。
    //外层循环遍历所有桶，内层循环遍历所有线程。对于每个线程和桶，都会计算该本地桶在全局排序数组中的起始位置，并将其保存在start_k数组中。
    //这个计算方法是通过将之前已经处理的所有本地桶中元素的个数累加得到的，即temp_k。最后，temp_k的值就是全局排序数组中元素的总个数。
    int temp_k = 0;//
    ul start_k[num_threads][num_buckets];
    for ( ul j = num_buckets-1 ; j >= 0 ; j-- ) 
    {
        for ( int t = 0 ; t < num_threads ; t++ ) 
        {
            start_k[t][j] = temp_k;
            temp_k += local_buckets[t][j].size();//对每个桶树立那个进行汇总
        }
    }

    cout<<"计算rank"<<endl;
    #pragma omp parallel for schedule(static)
    for ( int t = 0 ; t < num_threads; t++ ) {
        for ( ul j = num_buckets-1 ; j >= 0 ; j-- ) 
        {
            const vector<ul>& current_bucket = local_buckets[t][j];
            int k = start_k[t][j];
            const size_t& size = current_bucket.size();
            for ( ul i = 0 ; i < size ; i++ ) {
                rank[ current_bucket[i] ] = k++;
            }
        }
    }

    for ( ul i = 0 ; i < num_threads ; i++ ) 
    {
        for ( unsigned int j = 0 ; j < num_buckets ; j++ ) 
        {
            local_buckets[i][j].clear();
        }
    }
    
    
    return rank;
}
