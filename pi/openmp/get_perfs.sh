#!/bin/sh

MAX_VAL=10000000000

out_filename=omp_perfs.json

rm -f $out_filename

# start json
echo "[" > $out_filename

i=10000

while [ $i -le $MAX_VAL ]
do
    echo "./pi_seq $i >> $out_filename"
    ./pi_omp $i >> $out_filename
    i=$((i + i/2))
    if [ $i -le $MAX_VAL ]
    then
        echo "," >> $out_filename
    fi
done

# end json
echo "]" >> $out_filename
