all: heaptest

heaptest: heaptest.cc radixheap.cc
	clang++ -std=c++11 -o heaptest -g -O3 heaptest.cc radixheap.cc -lrt -ltcmalloc -I../../shared/util

clean:
	rm heaptest