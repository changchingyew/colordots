#!/bin/bash

count=0
echo "count: $count"

for (( j = 0; j <= 2448-300; j += 300)); 
do 
    for (( i = 0; i <= 3264-300; i += 300)); 
    do 
        echo "{.left = $i, .top = $j},"
        (( count=$count+1 ))
    done
done

echo "count: $count"