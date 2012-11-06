#!/usr/bin/python
#-*- coding:utf-8 -*-

import json
import matplotlib.pyplot as plt

cpp_filename = "../cpp/par_perfs.json"
omp_filename = "omp_perfs.json"

cpp_dict = json.load(open(cpp_filename))
omp_dict = json.load(open(omp_filename))

cpp_n_iter = [v['nIterations'] for v in cpp_dict]
cpp_times = [v['time'] for v in cpp_dict]

omp_n_iter = [v['nIterations'] for v in omp_dict]
omp_times = [v['time'] for v in omp_dict]



plt.xlabel("Number of elements in the sum")
plt.ylabel("Execution time in seconds")
plt.title("Computation of Pi. Blue: C++11 threads, green: openMP C++")
plt.plot(cpp_n_iter, cpp_times, 'bo-', linewidth=2)
plt.plot(omp_n_iter, omp_times, 'go-', linewidth=2)

plt.show()
