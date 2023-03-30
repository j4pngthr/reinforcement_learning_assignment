#!/bin/sh

for f in ~/gitwork/reinforcement_learning_assignment/MAB_cpp/data/*.txt
do
    echo $f
    #awk 'NR%50==0' $f
    g++ changeTimeInterval.cpp $f
done
