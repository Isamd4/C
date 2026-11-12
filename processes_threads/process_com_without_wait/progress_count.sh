#!/bin/bash

i=0
N=5

while (( $i <= $N ));do
    echo "Progress: $((i*2*10))%";
    sleep 0.5;
    ((i++));
done;
echo "done";

