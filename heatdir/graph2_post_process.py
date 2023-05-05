import numpy as np
from io import StringIO
import matplotlib.pyplot as plt

resolution = np.array([500, 1500, 2500, 3500, 4500])

# For batch node
count = 0
file_batch = open('/home/diogenes/Efficient_lab/heatdir/results/graph_2/Ofast/log_graph2_batch_Ofast.txt', 'r')
lines_batch = file_batch.readlines()

tags_batch = []
res1_batch = []
res2_batch = []
res3_batch = []
res4_batch = []
res5_batch = []
while(count < len(lines_batch)):
    res1_batch.append(float(lines_batch[count+1].split(';')[2]))
    res2_batch.append(float(lines_batch[count+2].split(';')[2]))
    res3_batch.append(float(lines_batch[count+3].split(';')[2]))
    res4_batch.append(float(lines_batch[count+4].split(';')[2]))
    res5_batch.append(float(lines_batch[count+5].split(';')[2]))
    tags_batch.append(lines_batch[count])
    count += 6
data_batch = [res1_batch, res2_batch, res3_batch, res4_batch, res5_batch]

# MFlops = np.stack((res1_batch,res2_batch,res3_batch,res4_batch,res5_batch))
# lst = np.arange(1,len(tags_batch_batch))

# For login node
count = 0
file_login = open('/home/diogenes/Efficient_lab/heatdir/results/graph_2/Ofast/log_graph2_login_Ofast.txt', 'r')
lines_login = file_login.readlines()

tags_login = []
res1_login = []
res2_login = []
res3_login = []
res4_login = []
res5_login = []
while(count < len(lines_login)):
    res1_login.append(float(lines_login[count+1].split(';')[2]))
    res2_login.append(float(lines_login[count+2].split(';')[2]))
    res3_login.append(float(lines_login[count+3].split(';')[2]))
    res4_login.append(float(lines_login[count+4].split(';')[2]))
    res5_login.append(float(lines_login[count+5].split(';')[2]))
    tags_login.append(lines_login[count])
    count += 6
data_login = [res1_login, res2_login, res3_login, res4_login, res5_login]

# Plots
fig = plt.figure(figsize =(10, 7))

bp1 = plt.boxplot(data_batch, notch=False, widths=0.35, 
                 patch_artist=True, boxprops=dict(facecolor="C0"))
bp2 = plt.boxplot(data_login, notch=False, widths=0.35, 
                 patch_artist=True, boxprops=dict(facecolor="C2"))
plt.xticks([1,2,3,4,5],['500','1500','2500','3500','4500'])
plt.xlabel('Resolution', fontsize = "10")
plt.ylabel('MFlop/s', fontsize = "10") 
plt.title('Group 6: Box Plots for batch and login node', fontsize = "10")
plt.legend([bp1["boxes"][0], bp2["boxes"][0]], ['Batch node', 'Login node'], loc='upper right')
plt.show()
