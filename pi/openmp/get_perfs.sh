#!/bin/sh

MAX_VAL=10000000000

#naif_filename=omp_naif_perfs.json
reduce_filename=omp_reduce_perfs_free_thread_nums.json

#naif_exe=./pi_omp_naif
reduce_exe=./pi_omp_reduce


rm -f $naif_filename $reduce_filename

# start json
echo "[" > $naif_filename
echo "[" > $reduce_filename

i=10000

while [ $i -le $MAX_VAL ]
do
    #echo "$naif_exe $i >> $naif_filename"
    #$naif_exe $i >> $naif_filename
    echo "$reduce_exe $i >> $reduce_filename"
    $reduce_exe $i >> $reduce_filename

    i=$((i + i/2))
    if [ $i -le $MAX_VAL ]
    then
        #echo "," >> $naif_filename
        echo "," >> $reduce_filename
    fi
done

# end json
#echo "]" >> $naif_filename
echo "]" >> $reduce_filename
