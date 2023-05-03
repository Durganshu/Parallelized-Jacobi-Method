# import numpy as np

# # read data from text file
# data = np.genfromtxt('log_trial.txt', delimiter=';', skip_header=1)

# # extract values into arrays
# resolution = data[:,0]
# time = data[:,1]
# MFlops = data[:,2]



import numpy as np
from io import StringIO

# read data from text file
with open('log_graph_1.txt') as f:
    data_str = f.read()

# split data into scenarios based on #CFLAGS
scenarios = data_str.split('#CFLAGS')[1:]

# extract MFlops from each scenario
MFlops = []
for scenario in scenarios:
    # skip the first line that contains the comment
    lines = scenario.strip().split('\n')[1:]
    # create a file-like object from the scenario string
    scenario_str = '\n'.join(lines)
    scenario_file = StringIO(scenario_str)
    # use np.genfromtxt to read the data from the file-like object
    scenario_data = np.genfromtxt(scenario_file, delimiter=';', skip_header=0)
    MFlops.append(scenario_data[:,2])

# print the MFlops arrays
# for i, MFlops_scenario in enumerate(MFlops):
#     print(f'MFlops[{i}] = {MFlops_scenario}')

file1 = open('log_graph_1.txt', 'r')
Lines = file1.readlines()

count = 0
tags = []

while(count < len(Lines)):
    # x = re.search("^CFLAGS =.*\n$", Lines[i])
    tags.append(Lines[count])
    count += 7

# print(tags)

resolution = np.array([500, 1500, 2500, 3500, 4500])
# # plot the data
import matplotlib.pyplot as plt

for i in range(0, len(tags)):
    plt.plot( resolution, MFlops[i], label=tags[i])
    # plt.legend()
plt.show()    
# plt.plot(resolution, MFlops, label='MFlops')
# plt.xlabel('resolution')


