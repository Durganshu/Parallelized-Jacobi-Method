import numpy as np
from io import StringIO
import matplotlib.pyplot as plt

resolution = np.array([500, 1500, 2500, 3500, 4500])

file1 = open('/home/diogenes/Efficient_lab/heatdir/results/graph_1/log_graph_1.txt', 'r')
lines = file1.readlines()

# flags =
#0 %        ["CFLAGS = -O1 -qopt-report-annotate -qopt-report-phase=vec,loop"     
#1 %         "CFLAGS = -O2 -qopt-report-annotate -qopt-report-phase=vec,loop"
#2 %         "CFLAGS = -O3 -qopt-report-annotate -qopt-report-phase=vec,loop"
#3 %         "CFLAGS = -Ofast -qopt-report-annotate -qopt-report-phase=vec,loop"
#4 %         "CFLAGS = -O2 -xhost -qopt-report-annotate -qopt-report-phase=vec,loop"
#5 %         "CFLAGS = -O2 -xCORE-AVX512 -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
#6          "CFLAGS = -O2 -xCORE-AVX512 -qopt-zmm-usage=high -ipo -qopt-report-annotate -qopt-report-phase=vec,loop"
#7 %         "CFLAGS = -O2 -ipo -qopt-report-annotate -qopt-report-phase=vec,loop"
#8 %         "CFLAGS = -O2 -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 9         "CFLAGS = -O2 -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 10 %       "CFLAGS = -O2 -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop"
# 11        "CFLAGS = -O3 -xhost -qopt-report-annotate -qopt-report-phase=vec,loop"
# 12        "CFLAGS = -O3 -xCORE-AVX512 -qopt-report-annotate -qopt-report-phase=vec,loop"
# 13        "CFLAGS = -O3 -xCORE-AVX512 -ipo -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 14        "CFLAGS = -O3 -ipo -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 15 %       "CFLAGS = -O3 -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 16        "CFLAGS = -O3 -xCORE-AVX512 -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop"
# 17        "CFLAGS = -O3 -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop"
# 18        "CFLAGS = -Ofast -xhost -qopt-report-annotate -qopt-report-phase=vec,loop"
# 19        "CFLAGS = -Ofast -xCORE-AVX512 -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 20        "CFLAGS = -Ofast -xCORE-AVX512 -ipo -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 21        "CFLAGS = -Ofast -ipo -qopt-report-annotate -qopt-report-phase=vec,loop"
# 22        "CFLAGS = -Ofast -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 23 %       "CFLAGS = -Ofast -xCORE-AVX512 -fno-alias -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop"
# 24        "CFLAGS = -Ofast -fno-alias -qopt-report-annotate -qopt-report-phase=vec,loop"
# 25 %      "CFLAGS = -O0"
# 26         "CFLAGS = -O3 -xCORE-AVX512 -qopt-zmm-usage=high -qopt-report-annotate -qopt-report-phase=vec,loop]"

idx_to_keep = [0,1,2,3,4,5,7,8,10,15,23,25]

tags_kept = ['-O1','-O2','-O3','-Ofast','-O2 -xhost','-O2 -xCORE-AVX512','-O2 -ipo','-O2 -xCORE-AVX512 -fno-alias','-O2 -fno-alias','-O3 -xCORE-AVX512 -fno-alias','-Ofast -xCORE-AVX512 -fno-alias', '-O0']

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

MFlops = np.stack((res1,res2,res3,res4,res5))
lst = np.arange(1,len(tags))

plt.figure(figsize=(30,15))
plt.xlabel('Resolution', fontsize = "10")
plt.ylabel('MFlop/s', fontsize = "10")
plt.title('Group 6: Initial performance data vs different compiler flags', fontsize = "10")
for i,tag in enumerate(tags_kept):
    plt.plot(resolution, MFlops[:,i], label = tag)
    plt.legend(loc = "upper right", fontsize = "10")
plt.show()
