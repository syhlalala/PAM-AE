ifdef CLANG
CC = clang++ 
CFLAGS = -mcx16 -O2 -I../c++ -fcilkplus -ldl -std=c++11 -march=native
else
CC = g++
# works with g++ (GCC) 5.4.1
CFLAGS = -O2 -I../c++ -mcx16 -march=native -fcilkplus -std=c++11 -Wall
endif

interval_tree:	interval_tree.cpp
	$(CC) $(CFLAGS) -DNDEBUG interval_tree.cpp -o interval_tree

clean:
	rm -f interval_tree
