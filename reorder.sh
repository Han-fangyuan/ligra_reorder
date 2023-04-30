#!/bin/bash


#parameters, options, backups
#com-amazon Amazon0302 baidu-baike cit-patent flickr road-road-usa twitter web-Google web-Stanford  yahoo youtube
datasets=(WikiTalk)

orders=(original deg gorder hubcluster hubsort selective_hubsort ldg minla minloga rcm slashburn)


#对于每个图数据
for dataset in ${!datasets[*]}  #dataset是数组变量索引【!" 表示展开数组 "datasets" 中的所有索引】
do
	#输入图数据文件
    input=./datasets/${datasets[$dataset]}.txt 
    folder=./datasets/${datasets[$dataset]}-preprocess
    mkdir $folder
	
    if ! [ -f $input ]; then   
        echo "Warning: dataset ${datasets[$dataset]} does not exist."
        continue
    fi
    
    echo "*********************************${datasets[$dataset]}图数据***************************************************************"
    #对于每个重排序算法order
    for order in ${orders[*]}; do
        # 重排序
        echo  "${order}重排序算法"
        output=${folder}/${datasets[$dataset]}-${order}.el
		if [ -f "$output" ]; then
			echo "Edgelist already exists."
		else
            rank=${folder}/tmp-${order}.txt
            cmd_ord="./preprocess/ord $input $order -d -o $rank" #重排序命令，输出rank文件
            $cmd_ord
            #$cmd_ord | tee -a $reorder_time
           	 
            cmd_rank1="./preprocess/rankedges el cvgr $input $rank $output"
            $cmd_rank1
            cmd_rank2="./preprocess/rankedges el cintgr $input $rank $output"
            $cmd_rank2
		fi

	done
done




