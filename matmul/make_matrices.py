#!/usr/bin/python

# Make matrices, multiply them, and store the result


import sys
import numpy as np

def mat_tostring(mat_as_list, size):
    mat_as_str_list = [[str(x) for x in sl] for sl in mat_as_list]
    mat_str = str(size) + "\n" + "\n".join([" ".join(sublist) for sublist in mat_as_str_list])
    return mat_str

def to_file(filename, string):
    file = open(filename, "w")
    file.write(string)
    file.close()


if len(sys.argv) != 5:
    print("Usage: %s dimension mat1_filename mat2_filename mat_rslt_filename" % sys.argv[0])
    exit(1)


size = int(sys.argv[1])

mat1_fn = sys.argv[2]
mat2_fn = sys.argv[3]
mat_rslt_fn = sys.argv[4]

mat1 = 10 * np.random.random((size, size))
mat2 = 10 * np.random.random((size, size))


mat_rslt = np.dot(mat1, mat2)

mat1 = mat1.tolist()
mat2 = mat2.tolist()
mat_rslt = mat_rslt.tolist()


to_file(mat1_fn, mat_tostring(mat1, size))
to_file(mat2_fn, mat_tostring(mat2, size))
to_file(mat_rslt_fn, mat_tostring(mat_rslt, size))
