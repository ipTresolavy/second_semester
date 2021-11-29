#include <stdio.h>
#include <stdlib.h>

typedef char item;

typedef struct node
{
    item info;
    struct node* prox;

} celula;

celula *insereNoInicio(celula*, item);
void imprimelista (celula*);
celula* reverteListaLigada(celula*);

int main()
{
    celula* linkedList;
    short i;

    linkedList = malloc(sizeof(celula));
    linkedList = NULL;

    for(i = 'D'; i >= 'A'; i--)
        linkedList = insereNoInicio(linkedList, i);

    imprimelista(linkedList);
    linkedList = reverteListaLigada(linkedList);
    imprimelista(linkedList);

    return 0;
}

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

celula *insereNoInicio (celula * inicio, item x)
{
  celula *novo = malloc(sizeof(celula));
  novo->info = x;
  novo->prox = inicio;
  
  return novo;
}

void imprimelista (celula* inicio)
{
  celula* aponta = inicio;
  
  while (aponta != NULL)
  {
    printf("%c ",aponta->info);
    aponta = aponta->prox;
  }
  
  printf("\n");
} 