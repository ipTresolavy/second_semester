#include <stdio.h>
#include <stdlib.h>

typedef char item;

typedef struct node
{
    item info;
    struct node* prox;

} celula;

celula* reverteListaLigada(celula *inicio)
{
    celula *atual = inicio, 
        *anterior = NULL, 
        *proximo;

    while(atual != NULL)
    {
        proximo = atual->prox;
        atual->prox = anterior;
        anterior = atual;
        atual = proximo;
    }

    return anterior;
}