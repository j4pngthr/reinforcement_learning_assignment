#!/bin/bash

maxseed=10

seed=0
while [ $seed -le $maxseed ]; do # maxseed回ループを回す
	echo "$seed"
	python3 main.py 0.0 0.0
	python3 main.py 0.01 0.0
	python3 main.py 0.1 0.0
	python3 main.py 0.0 5.0

	seed=$(expr $seed + 1)
done
