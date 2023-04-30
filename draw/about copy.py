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
orders =['or' ,'deg', 'go' ,'hc' ,'hs' ,'sh'  ,'ldg' ,'la', 'loga' ,'rcm','sb']
D = len(orders)


# 创建 3x2 的子图
fig, axs = plt.subplots(4, 2)
fig.tight_layout(pad=0, w_pad=0, h_pad=.1)
x = np.arange(D)


# 绘制子图2——创建数据
data_average_AID_out = [
        
        #'or',   'deg',    'go' ,            'hc' ,   'hs' ,     'sh'  ,       'ldg' ,  'la',    'loga' ,                    'rcm','  sb'
        [111047,126475,72992.3,             135463, 142675,111047,             101812,100984,106155,                    84680.8,69394],      #com-amazon
        [853382,895876,973446,              1.19544e+06,1.17438e+06,853382,    1.16188e+06,1.03756e+06,1.12042e+06,     907086 , 879756  ],     #cit-patent
        [313.765,1345.83, 811.894,          1148.99,1345.83, 313.765,          2272.36,1891.63,2077.62,                 771.59,531.826 ],       #facebook
        [548864, 5.79297e+06,1.71958e+06,   5.2189e+06,5.2161e+06,548864,      5.55901e+06,3.00801e+06,2.84894e+06,     2.98062e+06,2.68448e+06],   #road-road-usa
        [275291,263324, 262122,             289113, 293514,275291,             299532,254748, 273788,                    230784,269574],        #twitter  
        [416918, 312814,284612,             356465,361456, 416918,             353787,235168,262674,                     255557,340799],        #web-google
        [106033, 146853, 128015,            133182, 137319,106033,             162060,130051,139300,                     126950,71879.2]         #youtube      
]


#data_average_AID_out：original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[0, 0].plot(data_average_AID_out[0],  color = '#14517C')
axs[1, 0].plot(data_average_AID_out[1],  color = '#2F7FC1')
axs[1, 1].plot(data_average_AID_out[2],  color = '#E7EFFA')
axs[2, 0].plot(data_average_AID_out[3],  color = '#96C37D')
axs[2, 1].plot(data_average_AID_out[4],  color = '#F1D77E')
axs[3, 0].plot(data_average_AID_out[5],  color = '#F1A00E')
axs[3, 1].plot(data_average_AID_out[6],  color = '#D76364')

axs[0, 0].set_xticks(x)
axs[1, 0].set_xticks(x)
axs[1, 1].set_xticks(x)
axs[2, 0].set_xticks(x)
axs[2, 1].set_xticks(x)
axs[3, 0].set_xticks(x)
axs[3, 1].set_xticks(x)
axs[0, 0].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[1, 0].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[1, 1].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[2, 0].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[2, 1].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[3, 0].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[3, 1].set_xticklabels(orders, ha="right", rotation_mode="anchor")




#右上角标题
legend_lines, legend_labels = axs[0,0].get_legend_handles_labels()
axs[0,1].axis('off')
axs[0,1].legend(legend_lines, legend_labels,
        loc="center", ncol=3, columnspacing=1,
        frameon=False, borderaxespad=0.,
        title=("gap_average_bandwidth_out"), title_fontsize=14, fontsize=11)


# 设置子图的标题和标签BellmanFord BC BFS Components PageRank PageRankDelta Radii
axs[0, 0].set_title('com-amazon')
axs[1, 0].set_title('cit-patent')
axs[1, 1].set_title('facebook')
axs[2, 0].set_title('road-road-usa')
axs[2, 1].set_title('twitter')
axs[3, 0].set_title('web-google')
axs[3, 1].set_title('youtube')


#plt.setp(axs[:, 1], yticklabels=[])

# 调整布局
plt.tight_layout()

# 显示图形
plt.show()
