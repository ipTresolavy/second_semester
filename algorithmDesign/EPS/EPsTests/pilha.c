#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

pilha* criaPilha()
{
    pilha* p;
    p = malloc(sizeof(pilha));
    p->max = 1;
    p->topo = 0;
    p->v = malloc((p->max)*sizeof(item));
    return p;
}

void destroiPilha(pilha * p)
{
    free(p->v);
    free(p);
}

void realocaPilha(pilha *p)
{
    item * v2;
    int i; 
    v2 = malloc(2 * p->max * sizeof(item));
    for (i = 0; i < p->topo; i++)
        v2[i] = p->v[i];
    free (p->v);
    p->v = v2;
    p->max = 2 * p->max;
}

void empilha(pilha * p, item i)
{
    if(p->topo == p->max)
        realocaPilha(p);
    
    p->v[(p->topo)++] = i;
}

item desempilha(pilha * p)
{
    item aux;
    aux.l = EOF;
    aux.c = EOF;
    aux.dir = EOF;
    aux.lDest = EOF;
    aux.cDest = EOF;

    if(!pilhaVazia(p))
        aux = p->v[--(p->topo)];
    else
        printf("Deu ruim ao desempilhar\n");

    return aux;
}

int pilhaVazia(pilha * p)
{
    return (p->topo == 0);
}

item topoDaPilha(pilha * p)
{
    item aux;
    aux.l = EOF;
    aux.c = EOF;
    aux.dir = EOF;
    aux.lDest = EOF;
    aux.cDest = EOF;
    
    if(!pilhaVazia(p))
        aux = p->v[(p->topo) - 1];
    else
        printf("Deu ruim ao acessar o topo\n");

    return aux;
}