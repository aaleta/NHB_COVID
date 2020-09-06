CC= gcc
CFLAGS= -Wall -std=c99 -lm -lgsl -lgslcblas -O2 -Wno-unused-result
SOURCES=  main.c read.c utilities.c spread.c

.PHONY: seir
seir:
	$(CC) -DBASELINE $(SOURCES) $(CFLAGS) -o seir
