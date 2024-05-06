sequential = {}
lockfree = {}
coarse = {}
fine = {}

sequential['thread'] = [1]
sequential['put_time'] = [12.85842614]
sequential['search_time'] = [2.320922702]
sequential['time'] = [22.71773267]

coarse['thread'] = [1,2,4,8,16,32]
coarse['put_time'] = [12.47085957,17.2891837,17.44074103,18.36219396,18.62891124,19.16934821]
coarse['search_time'] = [2.359814334,3.179638922,3.302593636,3.602731093,3.49812651,3.36378212]
coarse['time'] = [22.16909673,28.14229077,32.0137458,33.67029446,35.482392,38.439247]

lockfree['thread'] = [1,2,4,8,16,32]
lockfree['put_time'] = [8.92333713,5.123697554,2.757549109,1.596539301,1.03191279,0.8235232]
lockfree['search_time'] = [1.85189829,1.045951654,0.582635532,0.344158557,0.2003497,0.1312481]
lockfree['time'] = [16.45728301, 8.45684493, 4.592635259, 2.752639771, 1.9121972, 1.3190238]

fine['thread'] = [1,2,4,8, 16, 32]
fine['put_time'] = [3.843026251,3.741108927,2.265032772,2.421893536,2.9347412,2.81257412]
fine['search_time'] = [2.055489293,1.314485173,0.853811693,1.303631828,1.4153790,1.510924]
fine['time'] = [11.83660371,7.436358739,5.33085414,5.598674878,5.12591712,5.9125072]

fine['put_time_2'] = [2.353498144,1.424445064]
fine['search_time_2'] = [1.571265596,0.909838724]
fine['time_2'] = [8.690343286,4.494627817,2.647618271,1.549079731]

lockfree['put_speedup'] = [lockfree['put_time'][0]/lockfree['put_time'][i] for i in range(6)]
lockfree['search_speedup'] = [lockfree['search_time'][0]/lockfree['search_time'][i] for i in range(6)]
lockfree['speedup'] = [lockfree['time'][0]/lockfree['time'][i] for i in range(6)]

coarse['put_speedup'] = [coarse['put_time'][0]/coarse['put_time'][i] for i in range(6)]
coarse['search_speedup'] = [coarse['search_time'][0]/coarse['search_time'][i] for i in range(6)]
coarse['speedup'] = [coarse['time'][0]/coarse['time'][i] for i in range(6)]

fine['put_speedup'] = [fine['put_time'][0]/fine['put_time'][i] for i in range(6)]
fine['search_speedup'] = [fine['search_time'][0]/fine['search_time'][i] for i in range(6)]
fine['speedup'] = [fine['time'][0]/fine['time'][i] for i in range(6)]

import matplotlib.pyplot as plt
import numpy as np

# Data setup
x = coarse['thread']  # All methods use the same thread counts
ideal_speedup = np.array(x)  # Ideal speedup is linear with the number of threads

# Plots
fig, axes = plt.subplots(nrows=3, ncols=1, figsize=(10, 15))

# Put Speedup Graph
axes[0].plot(x, coarse['put_speedup'], marker='o', linestyle='-', color='b', label='Coarse Put Speedup')
axes[0].plot(x, fine['put_speedup'], marker='o', linestyle='-', color='r', label='Fine Put Speedup')
axes[0].plot(x, lockfree['put_speedup'], marker='o', linestyle='-', color='g', label='Lock-Free Put Speedup')
axes[0].plot(x, ideal_speedup, marker='', linestyle='--', color='k', label='Ideal Linear Speedup')
axes[0].set_xlabel('Thread Count')
axes[0].set_ylabel('Put Speedup')
axes[0].set_title('Speedup Under Put-Intensive Workload')
axes[0].legend()
axes[0].grid(True)

# Search Speedup Graph
axes[1].plot(x, coarse['search_speedup'], marker='o', linestyle='-', color='b', label='Coarse Search Speedup')
axes[1].plot(x, fine['search_speedup'], marker='o', linestyle='-', color='r', label='Fine Search Speedup')
axes[1].plot(x, lockfree['search_speedup'], marker='o', linestyle='-', color='g', label='Lock-Free Search Speedup')
axes[1].plot(x, ideal_speedup, marker='', linestyle='--', color='k', label='Ideal Linear Speedup')
axes[1].set_xlabel('Thread Count')
axes[1].set_ylabel('Search Speedup')
axes[1].set_title('Speedup Under Get-Intensive Workload')
axes[1].legend()
axes[1].grid(True)

# Total Speedup Graph
axes[2].plot(x, coarse['speedup'], marker='o', linestyle='-', color='b', label='Coarse Total Speedup')
axes[2].plot(x, fine['speedup'], marker='o', linestyle='-', color='r', label='Fine Total Speedup')
axes[2].plot(x, lockfree['speedup'], marker='o', linestyle='-', color='g', label='Lock-Free Total Speedup')
axes[2].plot(x, ideal_speedup, marker='', linestyle='--', color='k', label='Ideal Linear Speedup')
axes[2].set_xlabel('Thread Count')
axes[2].set_ylabel('Total Speedup')
axes[2].set_title('Speedup Under 50-50 Put and Get Workload')
axes[2].legend()
axes[2].grid(True)

# Adjust layout
plt.tight_layout()
# plt.show()

import os
save_path = os.path.expanduser('~/Downloads/speedup_comparison.png')
plt.savefig(save_path)
# plt.savefig('home/zhinanwang/Downloads/speedup_comparison.png')