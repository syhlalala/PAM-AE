#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

export THREADS=`nproc --all`

printf "${GREEN}test interval tree${NC}\n"
printf "(Results in Table 4, first two rows)\n"
make

printf "${BLUE}parallel run${NC}\n"
export CILK_NWORKERS=$THREADS
numactl -i all ./interval_tree 100000000 100000000 7

echo
printf "${BLUE}sequential run${NC}\n"
export CILK_NWORKERS=1
./interval_tree 100000000 100000000 3
