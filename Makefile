CC = gcc
CFLAGS = -Wall -g
OBJ = main.o products.o accesses.o index.o

all: main

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

main.o: main.c products.h accesses.h index.h
products.o: products.c products.h
accesses.o: accesses.c accesses.h
index.o: index.c index.h

clean:
	rm -f *.o main
