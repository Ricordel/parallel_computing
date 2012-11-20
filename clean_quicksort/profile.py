#!/usr/bin/python
#-*- coding:utf-8 -*-

import json
import matplotlib.pyplot as plt
import sys


colors = ['blue', 'green', 'red', 'black', 'orange', 'brawn', 'purple', 'yellow']

# get file names
if len(sys.argv) < 2:
    print("Usage: %s file [file...]" % sys.argv[0])
    exit(1)
else:
    filenames = sys.argv[1:]

correspondance_color_files = ""

for i, fname in enumerate(filenames):
    d = json.load(open(fname))
    n_iter = [v['nElems'] for v in d]
    times = [v['time'] for v in d]

    plt.plot(n_iter, times, color=colors[i], linestyle='-', marker='o', linewidth=2, label=fname)



plt.xlabel("Number of elements in the sum")
plt.ylabel("Execution time in seconds")
plt.title("Quicksort\n" + correspondance_color_files)
plt.legend(loc='best', prop={'size':12})

plt.show()
