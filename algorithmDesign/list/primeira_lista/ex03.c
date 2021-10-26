/*
Resultados:

LGAROMOTI => Não é possível
ALGORITMO => A*L*G*O*R*I*T*M*O*
OMTIROGLA => OMTIROGLA*********
OTAIGLROM => OTA*IGL**RO****M**

*/
#include <stdio.h>
#include <stdlib.h>
#include "pilha03.h" /*pilha com item tipo char*/

#define ALGORITMO "ALGORITMO"

void sortWord(char, pilha*, pilha*);

int main()
{
    char seq[10]; /* caracteres em "ALGORITMO" + '\0'*/
    int i;
    pilha *algoStack = criaPilha(), 
          *algoResultStack = criaPilha();
    /*Ambas as pilhas são equivalentes. A diferença é que, ao invés de ser 
    desempilhada, a algoResultStack será empilhada com um '*' toda vez que 
    a algoStack for desempilhada, para ser printada na tela posteriormente*/   

    printf("\nInsira o anagrama de \"ALGORITMO\": ");

    if(scanf("%[^\n]", seq))
        for(i = 0; seq[i] != '\0'; ++i)
            sortWord(seq[i], algoStack, algoResultStack);
    else
        printf("\nHouve um problema com o input.\nFavor verificá-lo.\n\n");


    if(pilhaVazia(algoStack))
        for(i = 0; i < algoResultStack->topo; ++i)
            printf("%c", algoResultStack->v[i]);
    else
        printf("\nNão é possível obter ALGORITMO partindo desse anagrama.");
    
    printf("\n");
    destroiPilha(algoStack);
    destroiPilha(algoResultStack);

    return 0;
}

void sortWord(char charInput, pilha *algoStack, pilha * algoResultStack)
{
    static short c; /* Contador de caracteres na palavra "ALGORITMO" */
    /* Variáveis estáticas são inicializadas com 0 automaticamente */


    /* ALGORITMO[c] se refere a cada letra da palavra "ALGORITMO" */
    if(charInput == ALGORITMO[c])
    {
        empilha(algoResultStack, ALGORITMO[c++]);
        empilha(algoResultStack, '*');

        while(!pilhaVazia(algoStack) && topoDaPilha(algoStack) == ALGORITMO[c])
        {
            desempilha(algoStack);
            empilha(algoResultStack, '*');
            c++;
        }
    }
    else
    {
        empilha(algoResultStack, charInput);
        empilha(algoStack, charInput);
    }
}