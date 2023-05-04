import numpy as np
from io import StringIO
import matplotlib.pyplot as plt

resolution = np.array([500, 1500, 2500, 3500, 4500])

file1 = open('log_graph_1.txt', 'r')
lines = file1.readlines()

count = 0
tags = []
res1 = []
res2 = []
res3 = []
res4 = []
res5 = []
while(count < len(lines)):
    res1.append(float(lines[count+1].split(';')[2]))
    res2.append(float(lines[count+2].split(';')[2]))
    res3.append(float(lines[count+3].split(';')[2]))
    res4.append(float(lines[count+4].split(';')[2]))
    res5.append(float(lines[count+5].split(';')[2]))
    tags.append(lines[count])
    count += 7

less = []
mid = []
high = []
for i in range(0,len(res5)):
    if(res5[i] < 2000):
        less.append(i)
    if(res5[i] > 2000 and res5[i] < 3000):
        mid.append(i)
    if(res5[i] > 3000):
        high.append(i)

print('**************************LOW**********************************')
for idx in less:
    print(tags[idx])
    
print('***************************MID*********************************')
for idx in mid:
    print(tags[idx])
    
print('****************************HIGH********************************')
for idx in high:
    print(tags[idx])
    
MFlops = np.stack((res1,res2,res3,res4,res5))
lst = np.arange(1,len(tags))

for i in lst:
    plt.plot(resolution, MFlops[:,i-1], label = lst[i-1])
    plt.legend(loc = "upper right", fontsize = "5")

plt.show()
