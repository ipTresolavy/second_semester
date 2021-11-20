# Ajuste os diretorios abaixo conforme o seu sistema
LIBDIR = /usr/local/lib
CFLAGS = -g -Wall -std=c99 -O0 -ansi -pedantic -Wextra
CC = gcc
###########################################################################

all: compAll

compAll: ep4.o
	$(CC) -o ep4_exec ep4.o

ep4.o: ep4.c
	$(CC) $(CFLAGS) ep4.c -c

run:
	./ep4_exec

test:
	bash tests.sh

clean:
	rm -rf *.o *_exec *.txt *.out
