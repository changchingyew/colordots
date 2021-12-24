#!/bin/bash

set -x
cd tests
display -resize 816x612 ../tests/silverfalls.bmp &
cd ../build

for (( i = 9; i >= 0; i--)); 
do 
    for (( j = 7; j >=0; j--)); 
    do 
        ((k=$i + $j*10))
        ((x=75*$i + 1000))
        ((y=75*$j + 200))
        display -resize 75x75 -geometry +$x+$y silverfallscrop$k.bmp &
        sleep 1
    done
done
