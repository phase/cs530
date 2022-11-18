CC=gcc
CFLAGS=-I.

.DEFAULT_GOAL := all
.PHONY : all
all : main

main : v3math.c v3math.h main.c
	$(CC) $(CFLAGS) -o raytrace v3math.c main.c ppm.c

run : main
	./raytrace 1024 1024 Test/test2.scene Test/result.ppm

clean :
	rm raytrace
