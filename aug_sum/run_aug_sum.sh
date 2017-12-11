#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

printf "${GREEN}test aug_sum${NC}\n"
echo "(Results in Table 3)"

printf "${BLUE}parallel run${NC}\n"
./runall -r 7

echo
printf "${BLUE}sequential run${NC}\n"
./runall -r 3 -p 1
