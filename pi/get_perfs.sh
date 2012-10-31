#!/bin/sh

MAX_VAL_SEQ=1000000000 # More, it becomes unbeareable
MAX_VAL_PAR=10000000000

seq_filename=seq_perfs.json
par_filename=par_perfs.json

rm -f $seq_filename $par_filename

# start json
echo "[" > $seq_filename
echo "[" > $par_filename

i=10000

while [ $i -le $MAX_VAL_SEQ ]
do
    echo "./pi_seq $i >> $seq_filename"
    ./pi_seq $i >> $seq_filename
    i=$((i + i/2))
    if [ $i -le $MAX_VAL_SEQ ]
    then
        echo "," >> $seq_filename
    fi
done

echo "\n\n"

i=10000
while [ $i -le $MAX_VAL_PAR ]
do
    echo "./pi_parallel $i >> $par_filename"
    ./pi_parallel $i >> $par_filename
    i=$((i + i/2))
    if [ $i -le $MAX_VAL_PAR ]
    then
        echo "," >> $par_filename
    fi
done

# end json
echo "]" >> $seq_filename
echo "]" >> $par_filename
