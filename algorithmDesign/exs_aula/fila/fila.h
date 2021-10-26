#ifndef FILA
#define FILA
#include <stdio.h>

typedef struct
{
    int l;
    int c;
}pos;


typedef pos item;

typedef struct
{
    item *v;
    int ini;
    int fim;
    int max;
} fila;

fila* criaFila(int tam);
void destroiFila(fila *);
int filaVazia(fila *);
void aumentaFila(fila *);
item primeiroFila(fila *);
void insereFila(fila *, item);
item removeFila(fila *);

#endif