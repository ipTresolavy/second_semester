#include <stdio.h>
#include <stdlib.h>

typedef int item;

typedef struct node
{
    item info;
    struct node* prox;

} celula;

celula *insereNoInicio(celula*, item);
void imprimelista (celula*);
celula* intercalaListasLigadas(celula*, celula*);

int main()
{
    celula* finalLinkedList, *linkedList1, *linkedList2;

    linkedList1 = malloc(sizeof(celula));
    linkedList1 = NULL;

    linkedList2 = malloc(sizeof(celula));
    linkedList2 = NULL;

    linkedList2 = insereNoInicio(linkedList2, 9);
    linkedList2 = insereNoInicio(linkedList2, 7);
    linkedList2 = insereNoInicio(linkedList2, 5);
    linkedList2 = insereNoInicio(linkedList2, 3);
    linkedList2 = insereNoInicio(linkedList2, 1);

    linkedList1 = insereNoInicio(linkedList1, 8);
    linkedList1 = insereNoInicio(linkedList1, 6);
    linkedList1 = insereNoInicio(linkedList1, 4);
    linkedList1 = insereNoInicio(linkedList1, 2);
    linkedList1 = insereNoInicio(linkedList1, 0);
    
    finalLinkedList = intercalaListasLigadas(linkedList1, linkedList2);

    imprimelista(finalLinkedList);

    return 0;
}

celula* intercalaListasLigadas(celula* listaLigada1, celula* listaLigada2)
{

    celula* inicio = NULL,
    *fim,
    *aux = NULL;

    /* se a listaLigada1 não estiver vazia e a primeira informação da lista for menor ou igual à primeira informação da listaLigada2 */
    if (listaLigada1 != NULL && listaLigada1->info <= listaLigada2->info) 
    {
        /* inicio da lista intercalada recebe inicio da listaLigada1 */
        inicio = listaLigada1;

        /* faz a listaLigada1 começar no segundo elemento */
        listaLigada1 = listaLigada1->prox;
    }
    /* se a condição anterior falhar e a listaLigada2 não estiver vazia */
    else if(listaLigada2 != NULL)
    {
        /* inicio da lista intercalada recebe inicio da listaLigada2 */
        inicio = listaLigada2;

        /* faz a listaLigada2 começar no seu segundo elemento */
        listaLigada2 = listaLigada2->prox;
    }

    /* começando em 'inicio', se 'fim' não for nulo e enquanto nenhuma das listas tiver chegado no fim: compara dois elementos da lista, decide qual é o maior, ou se são iguais, e coloca o menor (ou igual) na lista ligada intercalada. 'fim' aponta para algum elemento na lista ligada 'inicio'. Alterar os valores apontados por fim, portanto, é alterar a lista apontada por 'inicio', sem alterar o valor de 'inicio'. */
    for(fim = inicio; fim != NULL && listaLigada1 != NULL && listaLigada2 != NULL; fim = aux) 
    {
        if (listaLigada1->info <= listaLigada2->info) 
        {
            /* aux recebe elemento menor ou igual entre listaLigada1 e listaLigada2 */
            aux = listaLigada1;

            /* prepara para análise na próxima iteração */
            listaLigada1 = listaLigada1->prox;
        }
        else 
        {
            /* aux recebe elemento menor ou igual entre listaLigada1 e listaLigada2 */
            aux = listaLigada2;

            /* prepara para análise na próxima iteração */
            listaLigada2 = listaLigada2->prox;
        }

        /* posição atualmente apontada por 'fim', localizada na lista 'inicio', é atualizada */
        fim->prox = aux;
    }

    /* recebe, se houver, ponteiro para sublista ligada que não foi analisada, pois a outra lista já foi inteiramente analisada. */
    if (listaLigada1 != NULL) 
        fim->prox = listaLigada1;

    else if (listaLigada2 != NULL) 
        fim->prox = listaLigada2;

    return inicio;
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
    printf("%d ",aponta->info);
    aponta = aponta->prox;
  }
  
  printf("\n");
} 
