import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
df = pd.read_excel('O3sheet.xlsx')

df = df.to_numpy()

resolution = df[:,0]
L2miss = df[:,1]
L3miss = df[:,2]
Mflopslikwid = df[:,5]
Mflopscode = df[:,6]

resolution = np.expand_dims(resolution, axis=1)
Mflopslikwid = np.expand_dims(Mflopslikwid, axis=1)
Mflopscode = np.expand_dims(Mflopscode, axis=1)

L2miss = np.transpose(L2miss)
L3miss = np.transpose(L3miss)


fig,ax = plt.subplots(figsize=(12, 6))

# make a plot
ax.plot(resolution,
        Mflopscode,
        label ="MFlops-code",
        color="red",
        marker="o")
ax.plot(resolution,
        Mflopslikwid,
        label ="MFlops-likwid",
        color="blue",
        marker="o")
# set x-axis label
ax.set_xlabel("Resolution", fontsize = 16)
# set y-axis label
ax.set_ylabel("Mflop/s",
              fontsize=16)
ax.legend(loc='center left')
plt.tick_params(axis='x', labelsize=16, width=4)
plt.tick_params(axis='y', labelsize=16, width=4)
# twin object for two different y-axis on the sample plot
ax2=ax.twinx()
# # make a plot with different y-axis using second axis object
ax2.plot(resolution, np.transpose([L2miss ,L3miss]), label =['L2 miss rate %','L3 miss rate %'], marker="o")
# ax2.set_yscale('log')
ax2.set_ylabel("L2, L3 miss rate %",fontsize=16)
plt.tick_params(axis='x', labelsize=16, width=4)
plt.tick_params(axis='y', labelsize=16, width=4)
ax2.legend(loc='center right')
plt.title('Group 6: -O3 -fno-alias -xhost', fontsize = 16)
plt.show()
# save the plot as a file
fig.savefig('O3_new.png',
            format='png',
            dpi=100,
            bbox_inches='tight')
fig.show()
# print(resolution)