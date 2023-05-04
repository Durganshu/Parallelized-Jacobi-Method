import numpy as np
from io import StringIO
import matplotlib.pyplot as plt

resolution = np.array([500, 1500, 2500, 3500, 4500])

file_batch = open('/home/diogenes/Efficient_lab/heatdir/results/graph_2/Ofast/log_graph2_batch_Ofast.txt', 'r')
# file_batch = open('/home/diogenes/Efficient_lab/heatdir/results/graph_2/Ofast/log_graph2_login_Ofast.txt', 'r')
lines_batch = file_batch.readlines()
# lines_login = file_login.readlines()

count = 0
tags = []
res1 = []
res2 = []
res3 = []
res4 = []
res5 = []
while(count < len(lines_batch)):
    res1.append(float(lines_batch[count+1].split(';')[2]))
    res2.append(float(lines_batch[count+2].split(';')[2]))
    res3.append(float(lines_batch[count+3].split(';')[2]))
    res4.append(float(lines_batch[count+4].split(';')[2]))
    res5.append(float(lines_batch[count+5].split(';')[2]))
    tags.append(lines_batch[count])
    count += 6
data = [res1, res2, res3, res4, res5]

    
MFlops = np.stack((res1,res2,res3,res4,res5))
lst = np.arange(1,len(tags))

# fig = plt.figure(figsize = (10,7))

data = [res1,res2,res3,res4,res5] 

fig = plt.figure(figsize =(10, 7))
# ax = fig.add_axes([0, 0, 1, 1])
 
# # Creating plot
plt.boxplot(data)
plt.xticks([1,2,3,4,5],['500','1500','2500','3500','4500'])
plt.xlabel('Resolution', fontsize = "10")
plt.ylabel('MFlop/s', fontsize = "10") 
plt.title('Group 6: Box Plot for batch node', fontsize = "10")
plt.show()
