CC = g++ 
CFLAGS = -O2 -I../c++ -mcx16 -march=native -fcilkplus -std=c++11 -Wall

index:	index.cpp index.h wiki_small.txt
	$(CC) $(CFLAGS) index.cpp -o index

wiki_small.txt: 
	bunzip2 -k wiki_small.txt.bz2

clean:
	rm -f index
