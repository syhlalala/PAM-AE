CC = g++ 
CFLAGS = -O2 -DNDEBUG -I../c++ -mcx16 -g -march=native -fcilkplus -std=c++11 -Wall

all: 	rt_test

rt_test:	rt_test.cpp range_tree.h
	$(CC) $(CFLAGS) rt_test.cpp -o rt_test

clean:
	rm -f rt_test
