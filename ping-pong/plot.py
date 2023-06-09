import pandas as pd
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 20})

# print("2\u00b3")
superscripts = {
    '0': '\u2070',
    '1': '\u00b9',
    '2': '\u00b2',
    '3': '\u00b3',
    '4': '\u2074',
    '5': '\u2075',
    '6': '\u2076',
    '7': '\u2077',
    '8': '\u2078',
    '9': '\u2079',
    '10': '\u00b9\u2070',
    '11': '\u00b9\u00b9',
    '12': '\u00b9\u00b2',
    '13': '\u00b9\u00b3',
    '14': '\u00b9\u2074',
    '15': '\u00b9\u2075',
    '16': '\u00b9\u2076',
    '17': '\u00b9\u2077',
    '18': '\u00b9\u2078',
    '19': '\u00b9\u2079',
    '20': '\u00b2\u2070',
    '21': '\u00b2\u00b9',
    '22': '\u00b2\u00b2',
    '23': '\u00b2\u00b3',
    '24': '\u00b2\u2074'
}

xlabels = [f"2{superscripts[str(i)]}" for i in range(25)]

# Read the Excel file
data1 = pd.read_csv('first_wala_thread.csv', header=None).to_numpy()
data2 = pd.read_csv('second_wala_core.csv', header=None).to_numpy()
data3 = pd.read_csv('third_wala_socket.csv', header=None).to_numpy()

# Extract the desired columns
column_0 = data1[:, 0]
column_1 = data1[:,1]
column_2 = data2[:,1]
column_3 = data3[:,1]



# #Plot the columns
# plt.plot(column_0, column_1, label='Processes in the same processor', linewidth=3)
# plt.plot(column_0, column_2, label='Same node, different processors', linewidth=3)
# plt.plot(column_0, column_3, label='Processes on different nodes', linewidth=3)
# ylabels = ['1000', '2000', '3000', '4000', '5000', '6000', '7000', '8000', '9000', '10000']  # Specify your desired labels
# plt.xticks(column_0, xlabels)
# plt.xlabel('Bytes')
# plt.ylabel('Mbytes/s')
# plt.title('Group 6: Machayenge')
# plt.legend()
# plt.grid(True)
# # Display the plot
# plt.show()

import matplotlib.pyplot as plt

# Plot the columns
plt.plot(column_0, column_1, label='Same socket', linewidth=3)
plt.plot(column_0, column_2, label='Across sockets', linewidth=3)
plt.plot(column_0, column_3, label='Across nodes', linewidth=3)

ylabels = [str(i) for i in range(0, 11000, 1000)]  # Generate y-axis labels at the interval of 1000

plt.yticks(range(0, 11000, 1000), ylabels)  # Set y-axis ticks and labels

plt.xticks(column_0, xlabels)  # Use existing x-axis ticks
plt.xlabel('Bytes')
plt.ylabel('Mbytes/s')
plt.title('Group 6: Bandwidth measurements')
plt.legend()
plt.grid(True)

# Display the plot
plt.show()
