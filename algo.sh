#!/bin/bash

perftool="perf stat -e task-clock,cache-misses,cache-references,L1-dcache-loads,L1-dcache-load-misses,L2_rqsts.miss"
#parameters, options, backups com-amazon
datasets=(com-amazon)
#orders=(original)
orders=(original deg gorder hubcluster hubsort selective_hubsort ldg minla minloga rcm  slashburn)
#orders=(original rand deg rcm gorder ldg dbg hubsort selective_hubsort hubcluster minla minloga slashburn)
algos=(BC)
#algos=(BellmanFord BC BFS Components PageRank PageRankDelta Radii)

dirn=$(date +%Y%m%d%H%M%S)
dirr=./results/r${dirn}
mkdir $dirr

#对于每个图数据
for dataset in ${!datasets[*]}  #dataset是数组变量索引【!" 表示展开数组 "datasets" 中的所有索引】
do
	


    folder=./datasets/${datasets[$dataset]}-preprocess
	
	
    
    #对于每个重排序算法order
    for algo in ${algos[*]};do
    	algo_time=${dirr}/${algo}.txt
       
        for order in ${orders[*]}
    	do
    		perf_time=${dirr}/${algo}-perf-$order.txt
            if [ "$algo" == "BellmanFord" ]; then
           		#tee -a $algo_time <<< "********************${datasets[$dataset]}-${order}-${algo}********************"
            	cmd_algo1=" $perftool -o ${perf_time} ./apps/$algo ${folder}/${datasets[$dataset]}-${order}.cintgr"
            	#cmd_algo1="$perftool -o $algo_time numactl -N all -i all ./apps/$algo ${folder}/${datasets[$dataset]}-${order}.cintgr"
            	${cmd_algo1} | tee -a $algo_time
                #${cmd_algo1}
               

            else
            	#tee -a $algo_time <<< "********************${datasets[$dataset]}-${order}-${algo} ********************"
                #cmd_algo2="./apps/$algo ${folder}/${datasets[$dataset]}-${order}.cvgr"
                cmd_algo2="$perftool -o ${perf_time} ./apps/$algo ${folder}/${datasets[$dataset]}-${order}.cvgr "
                ${cmd_algo2} | tee -a $algo_time
               
            fi
            
        done
	done
done




