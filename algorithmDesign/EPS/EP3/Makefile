# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /usr/local/lib 
CFLAGS = -Wall -O2 -ansi -pedantic -Wextra
CC = gcc-11
###########################################################################

all: compAll

compAll: ep3.o ep3_queimadas.o
	$(CC) -o ep3_exec ep3.o -lm
	$(CC) -o ep3_queimadas_exec ep3_queimadas.o -lm

ep3.o: ep3.c
	$(CC) $(CFLAGS) ep3.c -c

ep3_queimadas.o: ep3_queimadas.c
	$(CC) $(CFLAGS) ep3_queimadas.c -c

run_normal:
	./ep3_exec

run_burned:
	./ep3_queimadas_exec

test:
	bash tests.sh

clean:
	rm -rf *.o *_exec *.txt *.out