#!/bin/bash
make

export LARGE=0

while getopts "l" option
do
        case "${option}"
        in
          l) LARGE=1;;
        esac
done

cd aug_sum
chmod 777 run_aug_sum.sh
./run_aug_sum.sh $LARGE
echo "------------"

cd ../interval
chmod 777 run_interval.sh
./run_interval.sh
echo "------------"

cd ../range_query
chmod 777 run_range.sh
./run_range.sh
echo "------------"

cd ../index
chmod 777 run_index.sh
./run_index.sh
