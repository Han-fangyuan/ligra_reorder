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
        #'or',   'deg',    'go' ,   'hc' ,   'hs' ,     'sh'  ,   'ldg' ,  'la',    'loga' ,   'rcm','  sb'
        [111240,125836,73331.3,104229, 116039,111240, 94873.7, 98289.2,104252, 86381.2,76441.6],      #com-amazon
        [1.00025e+06,1.44229e+06, 1.38288e+06,1.51202e+06, 1.54151e+06, 1.00025e+06, 1.42293e+06,1.313e+06,1.44091e+06,1.34627e+06,1.06802e+06],     #cit-patent
        [826.054, 2294.77,1869.91,1977.49, 2294.77, 826.054, 2496.52, 1556.85,1854.39,1328.18, 894.254],        #facebook
        [552966, 5.3173e+06, 1.737e+06, 6.05838e+06,6.64699e+06, 552966, 5.39529e+06, 2.07806e+06, 1.82932e+06,3.72589e+06,2.99898e+06],   #road-road-usa
        [186171,164332, 174810, 196216, 202956,186171, 209618,110062, 138045, 128664,185603],        #twitter  
        [ 345805,260685,195089, 285369,274835,345805, 298740, 189350,211791, 150303, 250663],        #web-google
        [314857,505007,396464, 458954, 486440,314857,471027,361649, 397738, 336518, 124794]         #youtube      
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
        title=("gap_average_bandwidth_in"), title_fontsize=14, fontsize=11)


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
