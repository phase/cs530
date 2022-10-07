CC=gcc
CFLAGS=-I.

.DEFAULT_GOAL := all
.PHONY : all
all : main

main : v3math.c v3math.h v3test.c
	$(CC) $(CFLAGS) -o v3test v3math.c v3test.c

run :
	./v3test

clean :
	rm v3test
