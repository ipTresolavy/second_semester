#ifndef PILHA
#define PILHA

#include <stdio.h>

typedef char item;
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