sequential = {}
lockfree = {}
coarse = {}
fine = {}

sequential['thread'] = [1]
sequential['put_time'] = [12.85842614]
sequential['search_time'] = [2.320922702]
sequential['time'] = [22.71773267]

coarse['thread'] = [1,2,4,8]
coarse['put_time'] = [12.47085957,17.2891837,17.44074103,18.36219396]
coarse['search_time'] = [2.359814334,3.179638922,3.302593636,3.602731093]
coarse['time'] = [22.16909673,28.14229077,32.0137458,33.67029446]

lockfree['thread'] = [1,2,4,8]
lockfree['put_time'] = [8.92333713,5.123697554,2.757549109,1.596539301]
lockfree['search_time'] = [1.85189829,1.045951654,0.582635532,0.344158557]
lockfree['time'] = [16.45728301, 8.45684493, 4.592635259, 2.752639771]

fine['thread'] = [1,2,4,8]
fine['put_time'] = [3.843026251,3.741108927,2.265032772,2.421893536]
fine['search_time'] = [2.055489293,1.314485173,0.853811693,1.303631828]
fine['time'] = [11.83660371,7.436358739,5.33085414,5.598674878]

fine['put_time_2'] = [2.353498144,1.424445064]
fine['search_time_2'] = [1.571265596,0.909838724]
fine['time_2'] = [8.690343286,4.494627817,2.647618271,1.549079731]

lockfree['put_speedup'] = [lockfree['put_time'][0]/lockfree['put_time'][i] for i in range(4)]
lockfree['search_speedup'] = [lockfree['search_time'][0]/lockfree['search_time'][i] for i in range(4)]
lockfree['speedup'] = [lockfree['time'][0]/lockfree['time'][i] for i in range(4)]

coarse['put_speedup'] = [coarse['put_time'][0]/coarse['put_time'][i] for i in range(4)]
coarse['search_speedup'] = [coarse['search_time'][0]/coarse['search_time'][i] for i in range(4)]
coarse['speedup'] = [coarse['time'][0]/coarse['time'][i] for i in range(4)]

fine['put_speedup'] = [fine['put_time'][0]/fine['put_time'][i] for i in range(4)]
fine['search_speedup'] = [fine['search_time'][0]/fine['search_time'][i] for i in range(4)]
fine['speedup'] = [fine['time'][0]/fine['time'][i] for i in range(4)]

import matplotlib.pyplot as plt

# Plotting
# x = lockfree['thread']
# y1 = lockfree['put_speedup']
# y2 = lockfree['search_speedup']
# y3 = lockfree['speedup']
# x = coarse['thread']
# y1 = coarse['put_speedup']
# y2 = coarse['search_speedup']
# y3 = coarse['speedup']
x = fine['thread']
y1 = fine['put_speedup']
y2 = fine['search_speedup']
y3 = fine['speedup']

plt.plot(x, y1, marker='o', linestyle='-', color='b', label='Put Speedup')
plt.plot(x, y2, marker='o', linestyle='-', color='r', label='Search Speedup')
plt.plot(x, y3, marker='o', linestyle='-', color='g', label='Total Speedup')

# Adding labels, title, and legend
plt.xlabel('Thread Count')
plt.ylabel('Speedup')
# plt.title('Lockfree Speedup')
# plt.title('Coarse Speedup')
plt.title('Fine Speedup')
plt.legend()

# Display the graph
plt.grid(True)  # Optional, to add grid lines to the graph
plt.show()