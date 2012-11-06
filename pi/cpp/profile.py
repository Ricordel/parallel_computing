#!/usr/bin/python
#-*- coding:utf-8 -*-

import json
import matplotlib.pyplot as plt

seq_filename = "seq_perfs.json"
par_filename = "par_perfs.json"

seq_dict = json.load(open(seq_filename))
par_dict = json.load(open(par_filename))

# Keep only the ones for which two values are available, I've
# been further with the parallel
max_iter_entry = max(seq_dict, key=lambda x: x['nIterations'])
max_iter = max_iter_entry['nIterations']
par_dict = filter(lambda x: x['nIterations'] <= max_iter, par_dict)


seq_n_iter = [v['nIterations'] for v in seq_dict]
seq_times = [v['time'] for v in seq_dict]

par_n_iter = [v['nIterations'] for v in par_dict]
par_times = [v['time'] for v in par_dict]



plt.xlabel("Number of elements in the sum")
plt.ylabel("Execution time in seconds")
plt.title("Computation of Pi. Blue: monothread, green: 48 threads, red: monothread/48.")
plt.plot(seq_n_iter, seq_times, 'bo-', linewidth=2)
plt.plot(par_n_iter, par_times, 'go-', linewidth=3)
plt.plot(seq_n_iter, [t/48.0 for t in seq_times], 'ro-', linewidth=3)

plt.show()
