.PHONY: test

CC=gcc
INCLUDES=include
CFLAGS=-Wall -pipe -std=c99 -pedantic -I$(INCLUDES) -O2

all: goldeen.o main test.o

goldeen.o:
	$(CC) -o bin/goldeen.o -c $(CFLAGS) lib/goldeen/goldeen.c

main: 
	$(CC) -o bin/goldeen $(CFLAGS) bin/goldeen.o main.c

test.o: 
	$(CC) -o bin/test $(CFLAGS) bin/goldeen.o test/goldeen.c

test:
	@./bin/test
	@echo -e "\nIf all tests passed, we are ready to install goldeen. If you found any bug, \
please contact me via email (see the README file).\n\n\
If you want to go ahead and install it, just:\n\n\
\t# make install\n"

install:
	@cp bin/goldeen /usr/bin
	@echo goldeen installed.

clean:
	find -name "*~" | xargs rm -rf
	rm -f bin/*
