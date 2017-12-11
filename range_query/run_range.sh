#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

export THREADS=`nproc --all`

N=100000000
printf "${GREEN}test range queries${NC}\n"
echo "(Results in Table 4, row 3-5)"
make
printf "${RED}Query-sum:${NC}\n"
printf "${BLUE}Parallel run${NC}\n"
export CILK_NWORKERS=$THREADS
numactl -i all ./rt_test -n "$N" -q 1000000 -w 1000000000 -t 1
echo
printf "${BLUE}Sequential run${NC}\n"
export CILK_NWORKERS=1
./rt_test -n "$N" -q 1000000 -w 1000000000 -t 1

echo
printf "${RED}Query-all:${NC}\n"
printf "${BLUE}Parallel run${NC}\n"
export CILK_NWORKERS=$THREADS
numactl -i all ./rt_test -n "$N" -q 1000 -w 200000000 -t 0
echo
printf "${BLUE}Sequential run${NC}\n"
export CILK_NWORKERS=1
./rt_test -n "$N" -q 1000 -w 200000000 -t 0
