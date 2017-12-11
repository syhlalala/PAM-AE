#!/bin/bash
make

cd aug_sum
./run_aug_sum.sh
echo "------------"

cd ../interval
./run_interval.sh
echo "------------"

cd ../range_query
./run_range.sh
echo "------------"

cd ../index
./run_index.sh
