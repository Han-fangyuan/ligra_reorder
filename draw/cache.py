import matplotlib.pyplot as plt
import numpy as np
from matplotlib.font_manager import FontProperties

import numpy as np
import matplotlib.pyplot as plt
'''
7个数据集，10个重排序算法
'''

bar_width = 0.08
#图数据集
datasets=['am','cp','fb','tw','wg','yt']
D = len(datasets)


# 创建 3x2 的子图
fig, axs = plt.subplots(4, 2)
fig.tight_layout(pad=0, w_pad=0, h_pad=.1)
x = np.arange(D)







#子图1数据
data_BC = [
       #'am','cp','fb','rd','tw','wg','yt'
        [0.601 	,0.075 ,-0.284 	,0.689 	,0.109 	,-0.881 ],
        [2.500 	,0.121 ,4.686  	,1.043 	,0.744 	,-0.569 ],
        [-0.410 ,0.150 ,-0.049 	,0.424 	,0.183 	,-0.853 ],
        [-0.072 ,-0.436,0.000   	,0.833 	,0.203 	,-0.872 ],
        [-0.047 ,0.102 ,-0.049 	,0.424 	,0.145 	,0.033 ],
        [0.128 	,0.178 ,1.377  	,0.443 	,0.268 	,-0.127 ],
        [-0.121 ,0.153 ,3.079  	,0.535 	,17.490 ,-0.133 ],
        [0.280 	,0.078 ,0.758  	,0.713 	,7.987 	,-0.184 ],
        [1.011 	,-0.368,-0.049 	,1.034 	,0.686 	,-0.871 ],
        [0.556 	,0.244 ,10.600 	,0.409 	,0.392 	,0.033 ]
        ]

#子图2数据
data_SSSP = [
        #对应着每一个图
        [0.900 ,-0.603 ,1.535 	 ,0.191 	,0.150 	,-0.990 ],
        [1.500 ,2.723  ,59.952 	 ,0.373 	,1.073 	,-0.857 ],
        [-0.490,1.794  ,2.012    ,-0.397 ,-0.164 ,-0.987 ],
        [-0.680,-0.097 ,2.092 	 ,0.055 	,0.070 	,-0.990 ],
        [0.800 ,2.700  ,1.119 	 ,-0.234 ,0.214 	,0.040 ],
        [0.600 ,1.973  ,24.098 	 ,-0.194 ,-0.073 ,0.014 ],
        [0.300 ,2.306  ,38.752 	 ,0.059 	,14.315 ,-0.068 ],
        [0.230 ,1.995  ,15.410 	 ,0.122 	,114.038, 0.045 ],
        [-0.960,0.210  ,1.452 	 ,0.064 	,0.471 	,-0.990 ],
        [0.860 ,2.793  ,156.830  ,0.338 	,0.471 	,-0.008 ]
        ]

#子图3数据
data_BFS = [
        #对应着每一个图
       [0.130 	,0.688 	,-0.035 	,0.117 	,-0.126 	,-0.939 ],
       [0.260 	,1.047 	,8.083  	,1.017 	,0.316 	        , 0.028 ],
       [-0.770 	,1.342 	,-0.009 	,0.688 	,-0.059 	,-0.946 ],
       [-0.860 	,-0.672 ,-0.068 	,0.293 	,0.054 	        ,-0.932 ],
       [0.100 	,1.407 	,0.107 		,0.688 	,-0.217 	,71.185 ],
       [-0.120 	,1.430 	,1.861 		,0.676 	,0.074 	        , 61.464],
       [-0.220 	,0.646 	,3.561 		,0.803 	,1418.558 	,68.206 ],
       [0.130 	,1.321 	,1.639 		,0.630 	,1427.571 	,57.133 ],
       [-0.660 	,-0.395 ,0.000 		,1.183 	,0.253 	        ,-0.940 ],
       [0.210 	,1.430 	,25.914 	,0.889 	,0.197 	        ,67.987 ]

        ]

#子图4数据



data_Components = [
        #对应着每一个图
       [ -0.160 ,-0.509 ,-0.369 ,	0.381 ,	0.450 ,	-0.168], 
       [ 0.700 	,-0.384 ,-0.769 ,	0.720 ,	1.020 ,	-0.219], 
       [ -0.060 ,0.206 	,-0.073 ,	0.226 ,	0.103 ,	-0.030], 
       [ -0.140 ,-0.386 ,-0.578 ,	0.388 ,	0.150 ,	-0.080], 
       [ 0.290 	,0.192 	,-0.159 ,	0.068 ,	0.342 ,	0.090 ],
       [ 0.100 	,-0.614 ,-0.832 ,	-0.140, 0.081 ,	-0.609], 
       [ 0.240 	,-0.579 ,-0.757 ,	-0.032, 0.376 ,	-0.459], 
       [ 0.250 	,-0.603 ,-0.762 ,	-0.074, 0.134 ,	-0.275], 
       [ 1.010 	,-0.302 ,-0.296 ,	-0.210, 0.610 ,	-0.482], 
       [ 1.190 	,0.913 	,2.711 	,	0.235 ,	0.491 ,	1.419 ]

        ]









#子图5数据
data_PageRank = [
        #对应着每一个图
        [0.667,	-0.076,	0.100  ,0.307 ,	0.347 ,	0.071 ],
        [1.129,	0.496 ,	0.158  ,0.874 ,	2.709 ,	0.095 ],
        [0.168,	0.051 ,	0.092  ,0.265 ,	0.355 ,	0.204 ],
        [0.194,	-0.047,	0.141  ,0.262 ,	0.698 ,	0.127 ],
        [0.075,	0.024 ,	0.185  ,-0.066,0.194 ,	0.109 ],
        [0.029,	-0.379,	0.034  ,-0.149, 0.635 ,	-0.168],
        [-0.02, -0.344,	0.069  ,0.806 ,	0.620 ,	-0.211],
        [-0.03, -0.321,	-0.256 ,0.615 ,	0.559 ,	-0.157],
        [1.087,	0.341 ,	0.055  ,0.582 ,	2.423 ,	0.201 ],
        [0.257,	0.259 ,	0.124  ,0.153 ,	0.775 ,	0.201 ]

        ]

#子图6数据

data_PageRankDelta = [
        #对应着每一个图
       [-0.573,	0.061 	,0.024,	0.210,	0.185 ,	0.069 ],
       [0.669 ,	0.242 	,0.560,	0.236,	0.283 ,	0.167 ],
       [-0.368,	-0.069 	,0.222,	0.222,	0.170 ,	-0.079],
       [-0.540,	-0.141 	,0.119,	0.210,	0.158 ,	0.035 ],
       [0.238 ,	-0.076 	,0.065,	0.256,	0.202 ,	0.131 ],
       [0.451 ,	-0.265 	,0.079,	0.184,	0.269 ,	-0.042],
       [0.180 ,	-0.269 	,0.037,	0.139,	0.353 ,	0.077 ],
       [0.163 ,	-0.269 	,0.033,	0.152,	0.313 ,	0.039 ],
       [1.119 ,	0.119 	,0.100,	0.205,	0.363 ,	0.395 ],
       [0.394 ,	0.162 	,0.097,	0.099,	0.366 ,	0.242 ]

        ]

#子图7数据 





data_Radii= [
        #对应着每一个图
       [-0.794 ,0.304 ,	-0.037,  0.385 ,0.525 ,	0.348 ],
       [ 5.092 ,0.013 ,	0.094 ,  0.588 ,1.528 ,	1.975 ],
       [ -0.275, 0.143,-0.063,   0.159 ,0.257 ,	0.606 ],
       [ 0.107 ,-0.059, -0.055,  0.500 ,0.460 ,	0.013 ],
       [ 0.318 ,1.264 ,	0.122 ,  0.144 ,0.285 ,	0.558 ],
       [ 0.436 ,-0.155, -0.461,  0.205 ,0.239 ,	0.062 ],
       [ 0.549 ,-0.223, 0.01  ,  0.452 ,0.370 ,	-0.037],
       [ 0.229 ,0.519 ,	-0.071,  0.552 ,0.495 ,	-0.104],
       [ 4.179 ,1.603 ,	-0.175,  0.484 ,1.840 ,	0.935 ],
       [ 0.343 ,1.424 ,	0.071 ,  0.617 ,1.006 ,	1.042 ]

        ]

# 绘制子图1——BC算法——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[0, 0].bar(x + bar_width,    data_BC[0],   label = 'deg',               color = '#2F7FC1', width = bar_width)
axs[0, 0].bar(x + bar_width*2,  data_BC[1],   label = 'gorder',            color = '#E7EFFA', width = bar_width)
axs[0, 0].bar(x + bar_width*3,  data_BC[2],   label = 'hubcluster',        color = '#96C37D', width = bar_width)
axs[0, 0].bar(x + bar_width*4,  data_BC[3],   label = 'hubsort',           color = '#F1D77E', width = bar_width)
axs[0, 0].bar(x + bar_width*5,  data_BC[4],   label = 'selective_hubsort', color = '#A76364', width = bar_width)
axs[0, 0].bar(x + bar_width*6,  data_BC[5],   label = 'ldg',               color = '#D76364', width = bar_width)
axs[0, 0].bar(x + bar_width*7,  data_BC[6],   label = 'minla',             color = '#F7E1ED', width = bar_width)
axs[0, 0].bar(x + bar_width*8,  data_BC[7],   label = 'minloga',           color = '#C497B2', width = bar_width)
axs[0, 0].bar(x + bar_width*9,  data_BC[8],   label = 'rcm',               color = '#8ECFC9', width = bar_width)
axs[0, 0].bar(x + bar_width*10, data_BC[9],   label = 'slashburn',           color = '#A9B8C6', width = bar_width)
axs[0, 0].set_ylim([-1,1.5])
axs[0, 0].set_xticks(x+bar_width*5)
axs[0, 0].set_xticklabels(datasets, ha="right", rotation_mode="anchor")

# 绘制子图2——SSSP算法——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[1, 0].bar(x + bar_width,    data_SSSP[0],  label = 'deg',               color = '#2F7FC1', width = bar_width)
axs[1, 0].bar(x + bar_width*2,  data_SSSP[1],  label = 'gorder',            color = '#E7EFFA', width = bar_width)
axs[1, 0].bar(x + bar_width*3,  data_SSSP[2],  label = 'hubcluster',        color = '#96C37D', width = bar_width)
axs[1, 0].bar(x + bar_width*4,  data_SSSP[3],  label = 'hubsort',           color = '#F1D77E', width = bar_width)
axs[1, 0].bar(x + bar_width*5,  data_SSSP[4],  label = 'selective_hubsort', color = '#A76364', width = bar_width)
axs[1, 0].bar(x + bar_width*6,  data_SSSP[5],  label = 'ldg',               color = '#D76364', width = bar_width)
axs[1, 0].bar(x + bar_width*7,  data_SSSP[6],  label = 'minla',             color = '#F7E1ED', width = bar_width)
axs[1, 0].bar(x + bar_width*8,  data_SSSP[7],  label = 'minloga',           color = '#C497B2', width = bar_width)
axs[1, 0].bar(x + bar_width*9,  data_SSSP[8],  label = 'rcm',               color = '#8ECFC9', width = bar_width)
axs[1, 0].bar(x + bar_width*10, data_SSSP[9], label = 'slashburn',        color = '#A9B8C6', width = bar_width)
axs[1, 0].set_ylim([-1,1.5])
axs[1, 0].set_xticks(x+bar_width*5)
axs[1, 0].set_xticklabels(datasets, ha="right", rotation_mode="anchor")
   
# 绘制子图3——BFS算法——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[1, 1].bar(x + bar_width,    data_BFS[0],  label = 'deg',               color = '#2F7FC1', width = bar_width)
axs[1, 1].bar(x + bar_width*2,  data_BFS[1],  label = 'gorder',            color = '#E7EFFA', width = bar_width)
axs[1, 1].bar(x + bar_width*3,  data_BFS[2],  label = 'hubcluster',        color = '#96C37D', width = bar_width)
axs[1, 1].bar(x + bar_width*4,  data_BFS[3],  label = 'hubsort',           color = '#F1D77E', width = bar_width)
axs[1, 1].bar(x + bar_width*5,  data_BFS[4],  label = 'selective_hubsort', color = '#A76364', width = bar_width)
axs[1, 1].bar(x + bar_width*6,  data_BFS[5],  label = 'ldg',               color = '#D76364', width = bar_width)
axs[1, 1].bar(x + bar_width*7,  data_BFS[6],  label = 'minla',             color = '#F7E1ED', width = bar_width)
axs[1, 1].bar(x + bar_width*8,  data_BFS[7],  label = 'minloga',           color = '#C497B2', width = bar_width)
axs[1, 1].bar(x + bar_width*9,  data_BFS[8],  label = 'rcm',               color = '#8ECFC9', width = bar_width)
axs[1, 1].bar(x + bar_width*10, data_BFS[9],label = 'slashburn',         color = '#A9B8C6', width = bar_width)
axs[1, 1].set_ylim([-1,1.5])
axs[1, 1].set_xticks(x+bar_width*5)
axs[1, 1].set_xticklabels(datasets, ha="right", rotation_mode="anchor")

# 绘制子图4——Components算法——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[2, 0].bar(x + bar_width,    data_Components[0],  label = 'deg',               color = '#2F7FC1', width = bar_width)
axs[2, 0].bar(x + bar_width*2,  data_Components[1],  label = 'gorder',            color = '#E7EFFA', width = bar_width)
axs[2, 0].bar(x + bar_width*3,  data_Components[2],  label = 'hubcluster',        color = '#96C37D', width = bar_width)
axs[2, 0].bar(x + bar_width*4,  data_Components[3],  label = 'hubsort',           color = '#F1D77E', width = bar_width)
axs[2, 0].bar(x + bar_width*5,  data_Components[4],  label = 'selective_hubsort', color = '#A76364', width = bar_width)
axs[2, 0].bar(x + bar_width*6,  data_Components[5],  label = 'ldg',               color = '#D76364', width = bar_width)
axs[2, 0].bar(x + bar_width*7,  data_Components[6],  label = 'minla',             color = '#F7E1ED', width = bar_width)
axs[2, 0].bar(x + bar_width*8,  data_Components[7],  label = 'minloga',           color = '#C497B2', width = bar_width)
axs[2, 0].bar(x + bar_width*9,  data_Components[8],  label = 'rcm',               color = '#8ECFC9', width = bar_width)
axs[2, 0].bar(x + bar_width*10, data_Components[9],label = 'slashburn',         color = '#A9B8C6', width = bar_width)
axs[2, 0].set_ylim([-1,1.5])
axs[2, 0].set_xticks(x+bar_width*5)
axs[2, 0].set_xticklabels(datasets, ha="right", rotation_mode="anchor")

# 绘制子图5——PageRank算法——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[2, 1].bar(x + bar_width,    data_PageRank[0],  label = 'deg',               color = '#2F7FC1', width = bar_width)
axs[2, 1].bar(x + bar_width*2,  data_PageRank[1],  label = 'gorder',            color = '#E7EFFA', width = bar_width)
axs[2, 1].bar(x + bar_width*3,  data_PageRank[2],  label = 'hubcluster',        color = '#96C37D', width = bar_width)
axs[2, 1].bar(x + bar_width*4,  data_PageRank[3],  label = 'hubsort',           color = '#F1D77E', width = bar_width)
axs[2, 1].bar(x + bar_width*5,  data_PageRank[4],  label = 'selective_hubsort', color = '#A76364', width = bar_width)
axs[2, 1].bar(x + bar_width*6,  data_PageRank[5],  label = 'ldg',               color = '#D76364', width = bar_width)
axs[2, 1].bar(x + bar_width*7,  data_PageRank[6],  label = 'minla',             color = '#F7E1ED', width = bar_width)
axs[2, 1].bar(x + bar_width*8,  data_PageRank[7],  label = 'minloga',           color = '#C497B2', width = bar_width)
axs[2, 1].bar(x + bar_width*9,  data_PageRank[8],  label = 'rcm',               color = '#8ECFC9', width = bar_width)
axs[2, 1].bar(x + bar_width*10, data_PageRank[9],label = 'slashburn',         color = '#A9B8C6', width = bar_width)
axs[2, 1].set_ylim([-1,1.5])
axs[2, 1].set_xticks(x+bar_width*5)
axs[2, 1].set_xticklabels(datasets, ha="right", rotation_mode="anchor")

# 绘制子图6——PageRankDelta算法——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[3,0].bar(x + bar_width,    data_PageRankDelta[0],  label = 'deg',               color = '#2F7FC1', width = bar_width)
axs[3,0].bar(x + bar_width*2,  data_PageRankDelta[1],  label = 'gorder',            color = '#E7EFFA', width = bar_width)
axs[3,0].bar(x + bar_width*3,  data_PageRankDelta[2],  label = 'hubcluster',        color = '#96C37D', width = bar_width)
axs[3,0].bar(x + bar_width*4,  data_PageRankDelta[3],  label = 'hubsort',           color = '#F1D77E', width = bar_width)
axs[3,0].bar(x + bar_width*5,  data_PageRankDelta[4],  label = 'selective_hubsort', color = '#A76364', width = bar_width)
axs[3,0].bar(x + bar_width*6,  data_PageRankDelta[5],  label = 'ldg',               color = '#D76364', width = bar_width)
axs[3,0].bar(x + bar_width*7,  data_PageRankDelta[6],  label = 'minla',             color = '#F7E1ED', width = bar_width)
axs[3,0].bar(x + bar_width*8,  data_PageRankDelta[7],  label = 'minloga',           color = '#C497B2', width = bar_width)
axs[3,0].bar(x + bar_width*9,  data_PageRankDelta[8],  label = 'rcm',               color = '#8ECFC9', width = bar_width)
axs[3,0].bar(x + bar_width*10, data_PageRankDelta[9],label = 'slashburn',         color = '#A9B8C6', width = bar_width)
axs[3,0].set_ylim([-0.5,0.5])
axs[3,0].set_xticks(x+bar_width*5)
axs[3,0].set_xticklabels(datasets, ha="right", rotation_mode="anchor")


# 绘制子图7——Radii算法——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[3,1].bar(x + bar_width,    data_Radii[0],  label = 'deg',               color = '#2F7FC1', width = bar_width)
axs[3,1].bar(x + bar_width*2,  data_Radii[1],  label = 'gorder',            color = '#E7EFFA', width = bar_width)
axs[3,1].bar(x + bar_width*3,  data_Radii[2],  label = 'hubcluster',        color = '#96C37D', width = bar_width)
axs[3,1].bar(x + bar_width*4,  data_Radii[3],  label = 'hubsort',           color = '#F1D77E', width = bar_width)
axs[3,1].bar(x + bar_width*5,  data_Radii[4],  label = 'selective_hubsort', color = '#A76364', width = bar_width)
axs[3,1].bar(x + bar_width*6,  data_Radii[5],  label = 'ldg',               color = '#D76364', width = bar_width)
axs[3,1].bar(x + bar_width*7,  data_Radii[6],  label = 'minla',             color = '#F7E1ED', width = bar_width)
axs[3,1].bar(x + bar_width*8,  data_Radii[7],  label = 'minloga',           color = '#C497B2', width = bar_width)
axs[3,1].bar(x + bar_width*9,  data_Radii[8],  label = 'rcm',               color = '#8ECFC9', width = bar_width)
axs[3,1].bar(x + bar_width*10, data_Radii[9],  label = 'slashburn',         color = '#A9B8C6', width = bar_width)
axs[3,1].set_ylim([-1,1.5])
axs[3,1].set_xticks(x+bar_width*5)
axs[3,1].set_xticklabels(datasets, ha="right", rotation_mode="anchor")

# 设置每个子图的标题和标签BellmanFord BC BFS Components PageRank PageRankDelta Radii
axs[0, 0].set_title('BC')
axs[1, 0].set_title('BellmanFord')
axs[1, 1].set_title('BFS')
axs[2, 0].set_title('Components')
axs[2, 1].set_title('PageRank')
axs[3, 0].set_title('PageRankDelta')
axs[3, 1].set_title('Radii')


#绘制标题：把legend放在右上角，子图[0,1]位置
legend_lines, legend_labels = axs[0,0].get_legend_handles_labels()
axs[0,1].axis('off')
axs[0,1].legend(legend_lines, legend_labels,
        loc="center", ncol=3, columnspacing=1,
        frameon=False, borderaxespad=0.,
        title=("Ordering"), title_fontsize=14, fontsize=11)


plt.setp(axs[:, 0], ylabel='Speedup')
#plt.setp(axs[:, 1], yticklabels=[])

# 调整布局
plt.tight_layout()

# 显示图形
plt.show()
