#ifndef PILHA
#define PILHA

#include <stdio.h>

typedef struct
{
    int l;
    int c;
    int lDest;
    int cDest;
    int dir;
}pos;

typedef pos item;

typedef struct
{
    item *v;
    int topo;
    int max;

}pilha;

pilha * criaPilha();
void realocaPilha(pilha *);
item desempilha(pilha *);
item topoDaPilha(pilha *);
void destroiPilha(pilha *);
void empilha(pilha *, item);
int pilhaVazia(pilha *);

#endif