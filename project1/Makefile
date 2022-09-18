CC=gcc
CFLAGS=-I.

.DEFAULT_GOAL := all
.PHONY : all
all : main

main : main.c
	$(CC) $(CFLAGS) -o ppmrw main.c

run :
	./ppmrw