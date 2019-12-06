#!/usr/bin/bash

cd example_code

make clean
make

printf "solving the problem...\n"
for i in `seq 5`
do
num=`expr $i - 1`
./grpwk ../data/dat${num}_in ../data/dat${num}_out
done
printf "solved completely !\n"

cd ..
printf "now calc hensyukyori...\n"
python3 evaluate_all.py -d data

printf "executed completely !"

