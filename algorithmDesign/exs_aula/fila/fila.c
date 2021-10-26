#include <stdlib.h>
#include "fila.h"

fila* criaFila(int tam)
{
    fila *f = NULL;
    f = malloc(sizeof(fila));
    f->v = malloc(tam * sizeof(item));
    f->max = tam;
    f->ini = 0;
    f->fim = 0;

    return f;
}

void destroiFila(fila * f)
{
    free(f->v);
    free(f);
}

int filaVazia(fila * f)
{
    return (f->ini == f->fim);
}

void aumentaFila(fila * f)
{
    item *v2 = malloc(2 * f->max * sizeof(item));
    int i, k;

    for(i = f->ini, k = 0; i != f->fim; i = (i + 1)%f->max)    
        v2[k] = f->v[i];

    free(f->v);

    f->ini = 0;
    f->fim = f->max - 1;
    f->max *= 2;
    f->v = v2;
}

item primeiroFila(fila * f)
{
    if(!filaVazia(f))
        return f->v[f->ini];
    
}

void insereFila(fila * f, item X)
{
    if(f->ini == (f->fim + 1)%f->max) /* fila lotada */
        aumentaFila(f);

    f->v[f->fim] = X;
    f->fim = (f->fim + 1)%f->max;
}

item removeFila(fila * f)
{
    item aux;
    if(!filaVazia(f))
    {
        aux = f->v[f->ini];
        f->ini = (f->ini + 1)%f->max;
        return aux;
    }
}