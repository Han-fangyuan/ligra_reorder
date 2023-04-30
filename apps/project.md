编译个别算法可执行文件：7个


#算法
BellmanFord BC BFS Components PageRank PageRankDelta Radii
#重排序 
original rand deg rcm gorder ldg hubsort selective_hubsort hubcluster dbg minla minloga slashburn


g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o PageRank PageRank.C
g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o PageRankDelta PageRankDelta.C
g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o BC BC.C
g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o BFS BFS.C
g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o BellmanFord BellmanFord.C【】
g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o Components Components.C
g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o Radii Radii.C



g++ -std=c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o BellmanFord BellmanFord.C

运行：
numactl -N all -i all ./PageRank ../datasets/web-Google.cvgr
numactl -N all -i all ./PageRankDelta ../datasets/web-Google.cvgr
numactl -N all -i all ./BC -r 8 ../datasets/web-Google.cvgr
numactl -N all -i all ./BFS -r 8 ../datasets/web-Google.cvgr
numactl -N all -i all ./BellmanFord -r 8 ../datasets/web-Google.cintgr【】
numactl -N all -i all ./Components ../datasets/web-Google.cvgr
numactl -N all -i all ./Radii ../datasets/web-Google.cvgr
CF：numactl -N all -i all ./CF ../datasets/web-Google.cvgr
BF:需要权重



#算法
numactl -N all -i all ./PageRank ../datasets/web-Google.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-deg.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-gorder.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-hubcluster.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-hubsort.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-ldg.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-minla.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-minloga.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-original.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-rand.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-rcm.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-selective_hubsort.cvgr
numactl -N all -i all ./PageRank ../datasets/web-Google-slashburn.cvgr


numactl -N all -i all ./PageRank ../datasets/
numactl -N all -i all ./PageRank ../datasets/
numactl -N all -i all ./PageRank ../datasets/
numactl -N all -i all ./PageRank ../datasets/


需要root的算法：
bellmanford
BC
