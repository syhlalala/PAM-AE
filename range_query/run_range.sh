#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

export THREADS=`nproc --all`

head="numactl -i all"
if (hash numactl ls 2>/dev/null) 
then 
    head="numactl -i all" 
else 
    head="" 
fi

N=100000000
printf "${GREEN}test range queries${NC}\n"
echo "(Results in Table 4, row 3-5)"
make
printf "${RED}Query-sum:${NC}\n"
printf "${BLUE}Parallel run${NC}\n"
export CILK_NWORKERS=$THREADS
$head ./rt_test -n "$N" -q 1000000 -w 1000000000 -t 1 |tee res.txt
echo
printf "${BLUE}Sequential run${NC}\n"
export CILK_NWORKERS=1
./rt_test -n "$N" -q 1000000 -w 1000000000 -t 1  |tee -a res.txt

echo
printf "${RED}Query-all:${NC}\n"
printf "${BLUE}Parallel run${NC}\n"
export CILK_NWORKERS=$THREADS
$head ./rt_test -n "$N" -q 1000 -w 200000000 -t 0  |tee -a res.txt
echo
printf "${BLUE}Sequential run${NC}\n"
export CILK_NWORKERS=1
./rt_test -n "$N" -q 1000 -w 200000000 -t 0  |tee -a res.txt

python comp.py
