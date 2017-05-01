CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g
OBJ = lab26.o udt.o sort.o
PROG = program

build: lab26.o sort.o udt.o
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ)

lab26.o: lab26.c
	$(CC) $(CFLAGS) -c lab26.c

sort.o: sort.c sort.h
	$(CC) $(CFLAGS) -c sort.c

udt.o: udt.c udt.h
	$(CC) $(CFLAGS) -c udt.c

clean:
	rm $(PROG) $(OBJ)