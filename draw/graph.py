import matplotlib.pyplot as plt
import numpy as np
from matplotlib.font_manager import FontProperties

import numpy as np
import matplotlib.pyplot as plt
'''
7个数据集，10个重排序算法
'''

bar_width = 0.07
#图数据集
datasets=['am','cp','fb','rd','tw','wg','yt']
D = len(datasets)


# 创建 3x2 的子图
fig, axs = plt.subplots(4, 2)
fig.tight_layout(pad=0, w_pad=0, h_pad=.1)
x = np.arange(D)

# 创建数据1
data_Packing_Factor = [
        #对应着每一个图
        #'am',    'cp',    'fb',    'rd',       'tw',      'wg',        'yt'
        [2.4553,  2.61998, 2.41497,  2.15221,   3.13021,   2.52572,     3.7162],      #original排序
        [1.54621, 1.50966, 1.67347,  1.79856,   1.27486,   1.27998,     1.601],       #deg排序
        [2.37705, 2.53512, 2.29252,  2.82833,   2.24305,   1.80053,     3.27226],       #gorder排序
        [1,       1,       1.0068,   1,         1.00006,   1,           1.00005],       #hubcluster排序
        [1,       1.00001, 1.67347,  1,         1.00006,   1,           1.00005],       #hubsort排序
        [2.4553,  2.61998, 2.41497,  2.15221,   3.13021,   2.52572,     3.7162],       #selective_hubsort排序
        [2.72563, 2.75191, 3.17007,  2.76408,   3.21197,   2.47655,     5.18683],       #ldg
        [2.63747, 2.42506, 2.7483,   2.45992,   2.35511,   2.30282,     4.75863],       #minla
        [2.69512, 2.67534, 3.06803,  2.36381,   2.28311,   2.48289,     4.95854],       #minloga
        [2.01821, 1.86331, 2.04082,  2.07256,   2.04686,   1.62713,     2.53316],      #rcm
        [2.62763, 2.54003, 2.61224,  2.61923,   2.60198,    2.22548,    4.39999]       #slashburn
        ]

#Packing Factor——original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[0, 0].bar(x ,               data_Packing_Factor[0], label = 'original',         color = '#14517C', width = bar_width)
axs[0, 0].bar(x + bar_width,    data_Packing_Factor[1], label = 'deg',              color = '#2F7FC1', width = bar_width)
axs[0, 0].bar(x + bar_width*2,  data_Packing_Factor[2], label = 'gorder',           color = '#E7EFFA', width = bar_width)
axs[0, 0].bar(x + bar_width*3,  data_Packing_Factor[3], label = 'hubcluster',       color = '#96C37D', width = bar_width)
axs[0, 0].bar(x + bar_width*4,  data_Packing_Factor[4], label = 'hubsort',          color = '#F1D77E', width = bar_width)
axs[0, 0].bar(x + bar_width*4,  data_Packing_Factor[5], label = 'selective_hubsort',color = '#F1A00E', width = bar_width)
axs[0, 0].bar(x + bar_width*5,  data_Packing_Factor[6], label = 'ldg',              color = '#D76364', width = bar_width)
axs[0, 0].bar(x + bar_width*6,  data_Packing_Factor[7], label = 'minla',            color = '#F7E1ED', width = bar_width)
axs[0, 0].bar(x + bar_width*7,  data_Packing_Factor[8], label = 'minloga',          color = '#C497B2', width = bar_width)
axs[0, 0].bar(x + bar_width*8,  data_Packing_Factor[9], label = 'rcm',              color = '#8ECFC9', width = bar_width)
axs[0, 0].bar(x + bar_width*9,  data_Packing_Factor[10], label = 'slashburn',        color = '#A9B8C6', width = bar_width)
axs[0, 0].set_xticks(x+bar_width*5)
axs[0, 0].set_xticklabels(datasets, ha="right", rotation_mode="anchor")


#把legend放在右上角，子图[0,1]位置
legend_lines, legend_labels = axs[0,0].get_legend_handles_labels()
axs[0,1].axis('off')
axs[0,1].legend(legend_lines, legend_labels,
        loc="center", ncol=3, columnspacing=1,
        frameon=False, borderaxespad=0.,
        title=("Ordering"), title_fontsize=14, fontsize=11)

# 设置子图的标题和标签BellmanFord BC BFS Components PageRank PageRankDelta Radii
axs[0, 0].set_title('Packing Factor')
axs[1, 0].set_title('average AID_out')
axs[1, 1].set_title('average AID_in')
axs[2, 0].set_title('gap profile')
axs[2, 1].set_title('gap bandwidth')
axs[3, 0].set_title('gap average bandwidth_out')
axs[3, 1].set_title('gap average bandwidth_in')

plt.setp(axs[:, 0], ylabel='Speedup')
#plt.setp(axs[:, 1], yticklabels=[])

# 调整布局
plt.tight_layout()

# 显示图形
plt.show()
