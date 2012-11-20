#!/bin/bash

MAX_VAL=1000000

nitems_to_test="1000 10000 30000 60000 100000 300000 600000 1000000 3000000 6000000 10000000"
#nitems_to_test="1000 10000 30000 60000 100000 300000 600000 1000000 3000000"

# Takes the name of the executable as entry
function gather_perfs {
    exe=$1
    out_file=perfs_$1.dat
    echo "[" > $out_file
    for i in $nitems_to_test;
    do
        ./$exe $i | tee -a $out_file
        if [ $i -ne 10000000 ]; then
            echo "," >> $out_file # ERROR: ne doit pas être ajouté la dernière fois.
        fi
    done
    echo "]" >> $out_file
}


export GOMAXPROCS=4
gather_perfs qsort_seq
gather_perfs qsort_omp
gather_perfs qsort_go_seq
gather_perfs qsort_go_par
