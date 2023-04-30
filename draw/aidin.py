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
fig, axs = plt.subplots(3, 2)
fig.tight_layout(pad=0, w_pad=0, h_pad=.1)
x = np.arange(D)


# 绘制子图2——创建数据
data_average_AID_in = [
        
        #'or',   'deg',    'go' ,   'hc' ,   'hs' ,     'sh'  ,   'ldg' ,  'la',    'loga' ,   'rcm','  sb'
        [76522,   96029,    38099,   83782,   95853,    76522,    58722,   65992,    69271,    48650,   36422],      #com-amazon
        [284390,  492180,   357431,  437586,  465061,   284390,   389237,  305590,   338147,   305603,  62649] ,
        [599350,  1095648,  777474,  1157300, 1194712,  599350,   835793,  880805,   931844,   947996,  554134],     #cit-patent
        [279,     1135,     665,     1047,    1135,     279,      1327,    830,      935,      533,     340],        #facebook
        [96561,   96094,    89276,   106259,  113378,   96561,    119288,  54837,    66554,    63702,   112885],        #twitter  
        [239556,  209946,   113037,  217043,  216457,   239556,   196068,  133913,   144177,   83409,   160536]       #web-google
                #youtube      
]




#data_average_AID_out：original deg gorder hubcluster hubsort selective_hubsort  ldg minla minloga rcm slashburn
axs[0, 0].plot(data_average_AID_in[0],  color = '#14517C')
axs[0, 1].plot(data_average_AID_in[1],  color = '#2F7FC1')
axs[1, 0].plot(data_average_AID_in[2],  color = '#E7EFFA')
axs[1, 1].plot(data_average_AID_in[3],  color = '#96C37D')
axs[2, 0].plot(data_average_AID_in[4],  color = '#F1D77E')
axs[2, 1].plot(data_average_AID_in[5],  color = '#F1A00E')


axs[0, 0].set_xticks(x)
axs[0, 1].set_xticks(x)
axs[1, 0].set_xticks(x)
axs[1, 1].set_xticks(x)
axs[2, 0].set_xticks(x)
axs[2, 1].set_xticks(x)

axs[0, 0].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[0, 1].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[1, 0].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[1, 1].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[2, 0].set_xticklabels(orders, ha="right", rotation_mode="anchor")
axs[2, 1].set_xticklabels(orders, ha="right", rotation_mode="anchor")





# 设置子图的标题和标签BellmanFord BC BFS Components PageRank PageRankDelta Radii
axs[0, 0].set_title('com-amazon')
axs[0, 1].set_title('youtube')
axs[1, 0].set_title('cit-patent')
axs[1, 1].set_title('facebook')
axs[2, 0].set_title('twitter')
axs[2, 1].set_title('web-google')

#plt.setp(axs[:, 1], yticklabels=[])

# 调整布局
plt.tight_layout()

# 显示图形
plt.show()
