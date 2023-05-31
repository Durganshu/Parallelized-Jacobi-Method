import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 20})
# Read the Excel file
data = pd.read_excel('assignment4.2.xlsx')

# Extract the desired columns
x_values = ['1', '2', '4', '8', '12', '16', '24', '32', '48']
column3 = data['Speedup2000']
column4 = data['Speedup6000']

# Plot the columns
plt.plot(x_values, column3, label='Speedup 2000 Resolution', linewidth=3)
plt.plot(x_values, column4, label='Speedup 6000 Resolution', linewidth=3)
labels = ['1', '2', '4', '8', '12', '16', '24', '32', '48']  # Specify your desired labels
plt.xticks(x_values, labels)
plt.xlabel('Number of threads')
plt.ylabel('Speedup')
plt.title('Group 6: Automatic Parallelization')
plt.legend()
plt.grid(True)
# Display the plot
plt.show()
