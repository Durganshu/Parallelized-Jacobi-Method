import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

x_values = ['200', '1200', '2200', '3200', '4200']
scale = 100
df = pd.read_excel('assignment3.xlsx')

df = df.to_numpy()
L2percent_decrease = df[:,5]
time_decrease = df[:,6]

bar_width = 0.35
x_positions = np.arange(len(x_values))

fig, ax = plt.subplots()
ax.bar(x_positions, L2percent_decrease, bar_width, label='% of original L2 miss ratio')

ax.bar(x_positions + bar_width, time_decrease, bar_width, label='% of original time for execution')
ax.axhline(y=scale, color='r', linestyle='--')
ax.set_xticks(x_positions + bar_width / 2)
ax.set_xticklabels(x_values)
plt.xticks(fontsize=15)
plt.yticks(fontsize=15)
ax.set_ylabel('% of original')
ax.xaxis.get_label().set_fontsize(15)
ax.yaxis.get_label().set_fontsize(15)
ax.set_xlabel('Resolution')

ax.set_ylim([0,120])

ax.legend(prop = { "size": 15 })
plt.title("Group 6: Sequential optimization",fontsize=15)

plt.show()
fig.savefig('Sequential_optimization.png',
            format='png',
            dpi=100,
            bbox_inches='tight')
