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
        
        #'or',   'deg',    'go' ,   'hc' ,   'hs' ,   'sh'  ,   'ldg' ,  'la',    'loga' ,   'rcm','  sb'
        [76804,   95192,   52516,   84517,   92084,    76804,    68001,   66528,   68844,    57703,   52156],      #com-amazon
        [467325,  516681,  589213,  687244,  651396,   467325,   620587,  546493,  581520,   534351,  516859],     #cit-patent
        [170,     625,     409,     562,     625,      170,      1201,    839,     950,      386,     303],        #facebook
        [440364,  4817233, 1446002, 4496144, 4543561,  440364,   4463712, 2130947, 1979711,  2097977, 2153616],   #road-road-usa
        [147497,  197344,  193191,  148189,  149455,   147497,   157550,  104334,  114877,   173256,  212677],        #twitter  
        [253387,  248137,  202709,  210042,  208692,   253387,   212480,  125424,  137552,   174523,  271887],        #web-google
        [70990,   100072,  92330,   90277,   95090,    70990,    121695,  86780,   90036,    84387,   43331]         #youtube      
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
        title=("average_AID_out"), title_fontsize=14, fontsize=11)


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
