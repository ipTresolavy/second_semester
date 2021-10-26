# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /usr/local/lib 
CFLAGS = -Wall -O2 -ansi -pedantic -Wextra
###########################################################################

all: compAll

compAll: ep2.o pilha.o
	gcc -o ep2 ep2.o pilha.o -lm

ep2.o: ep2.c pilha.h
	gcc $(CFLAGS) ep2.c -c

pilha.o: pilha.c pilha.h
	gcc $(CFLAGS) pilha.c -c

test:
	./tests.sh

clean:
	rm -rf *.o *~ client


