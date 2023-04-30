#!/bin/bash


#parameters, options, backups
datasets=(web-Google)


orders=(original rand deg rcm gorder ldg hubsort selective_hubsort hubcluster minla minloga slashburn)
#orders=(original rand deg rcm gorder ldg dbg hubsort selective_hubsort hubcluster minla minloga slashburn)
algos=(BFS)
#algos=(BellmanFord BC BFS Components PageRank PageRankDelta Radii)

#dirn=$(date +%Y%m%d%H%M%S)
#dirr=./results/${dirn}


#对于每个图数据
for dataset in ${!datasets[*]}  #dataset是数组变量索引【!" 表示展开数组 "datasets" 中的所有索引】
do
	#输入图数据文件
    input=./datasets/${datasets[$dataset]}.txt 
    folder=./datasets/${datasets[$dataset]}-preprocess
    mkdir $folder
	re=./results/${datasets[$dataset]}
	mkdir $re

	
    if ! [ -f $input ]; then   
        echo "Warning: dataset ${datasets[$dataset]} does not exist."
        continue
    fi
    
    echo "**********************${datasets[$dataset]}图数据**********************"
    echo
    

    #对于每个重排序算法order
    for order in ${orders[*]}; do
        echo "运行${datasets[$dataset]}-${order}重排序"
        # 重排序
        output=${folder}/${datasets[$dataset]}-${order}.el
		if [ -f "$output" ]; then
			echo "Edgelist already exists."
		else
			#rank文件计算重排序时间./ord ../datasets/web-Google.txt original -d -o tmp-original.txt
            rank=${folder}/tmp-${order}.txt
            cmd_ord="./preprocess/ord $input $order -d -o $rank" #重排序命令，输出rank文件
            $cmd_ord
            #$cmd_ord | tee -a $reorder_time
           	 
			#排列数据，并且转换格式./rankedges el cvgr ../datasets/web-Google.txt tmp-original.txt ../datasets/web-Google-original.el
            cmd_rank1="./preprocess/rankedges el cvgr $input $rank $output"
            $cmd_rank1
            cmd_rank2="./preprocess/rankedges el cintgr $input $rank $output"
            $cmd_rank2
		fi
        #echo "********************${datasets[$dataset]}-${order}重排序结束********************"
	done
	
	 #对于每个重排序算法order
    for algo in ${algos[*]};do
    	algo_time=${re}/${algo}-$(date +%Y%m%d%H%M%S).txt
        for order in ${orders[*]}
    	do
    		
            if [ "$algo" == "BellmanFord" ]; then
            	cmd_algo1="./apps/$algo ${folder}/${datasets[$dataset]}-${order}.cintgr"
            	tee -a $algo_time <<< "********************${datasets[$dataset]}-${order}-${algo}********************"
            	${cmd_algo1} | tee -a $algo_time
            else
                cmd_algo2="./apps/$algo ${folder}/${datasets[$dataset]}-${order}.cvgr"
                tee -a $algo_time <<< "********************${datasets[$dataset]}-${order}-${algo} ********************"
                ${cmd_algo2} | tee -a $algo_time
            fi
            
        done
	done
	
done




