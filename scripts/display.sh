#!/bin/bash

set -x
cd tests
display -resize 816x612 ../tests/silverfalls.bmp &
cd ../build

for (( i = 9; i >= 0; i--)); 
do 
    for (( j = 7; j >=0; j--)); 
    do 
        ((crop_x=300*$i))
        ((crop_y=300*$j))
        ((x=$crop_x/4 + 100))
        ((y=$crop_y/4 + 100))
        display -resize 75x75 -geometry +$x+$y silverfalls-x${crop_x}y${crop_y}.bmp &
        sleep 1
    done
done
