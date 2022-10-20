CC=gcc
CFLAGS=-I.

.DEFAULT_GOAL := all
.PHONY : all
all : main

main : v3math.c v3math.h main.c
	$(CC) $(CFLAGS) -o raycast v3math.c main.c ppm.c

run : main
	./raycast Test/test.json Test/result.ppm

clean :
	rm raycast
