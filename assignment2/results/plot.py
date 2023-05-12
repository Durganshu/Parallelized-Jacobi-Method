import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
df = pd.read_excel('O2sheet.xlsx')

df = df.to_numpy()

resolution = df[:,0]
L2miss = df[:,1]
L3miss = df[:,2]
Mflopscode = df[:,5]

resolution = np.expand_dims(resolution, axis=1)
Mflopscode = np.expand_dims(Mflopscode, axis=1)

L2miss = np.transpose(L2miss)
L3miss = np.transpose(L3miss)


fig,ax = plt.subplots()
# make a plot
ax.plot(resolution,
        Mflopscode,
        label ="MFlops",
        color="red",
        marker="o")
# set x-axis label
ax.set_xlabel("Reoslution", fontsize = 14)
# set y-axis label
ax.set_ylabel("Mflop/s",
              fontsize=14)
ax.legend(loc='upper left')

# twin object for two different y-axis on the sample plot
ax2=ax.twinx()
# # make a plot with different y-axis using second axis object
ax2.plot(resolution, np.transpose([L2miss ,L3miss]), label =['L2 miss rate %','L3 miss rate %'], marker="o")
# ax2.set_yscale('log')
ax2.set_ylabel("L2, L3 miss rate %",fontsize=14)
ax2.legend(loc='center right')
plt.show()
# save the plot as a file
fig.savefig('O2.png',
            format='png',
            dpi=100,
            bbox_inches='tight')

# print(resolution)