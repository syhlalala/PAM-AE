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

printf "${GREEN}test inverted indices${NC}"
echo "(Results in Table 5, with smaller inputs)"
make
printf "${BLUE}parallel run${NC}\n"
export CILK_NWORKERS=$THREADS
$head ./index -f "wiki_small.txt" -q 100000 | tee res.txt
echo
printf "${BLUE}sequential run${NC}\n"
export CILK_NWORKERS=1
./index -f "wiki_small.txt" -q 100000 |tee -a res.txt

python comp.py