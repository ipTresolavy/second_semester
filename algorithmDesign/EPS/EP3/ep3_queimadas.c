/**
 * @file ep3_queimadas.c
 * @author Igor Pontes Tresolavy (tresolavy@usp.br)
 * @brief algoritmo que ordena pilha de panquecas com diâmetros diferentes e lados queimados
 * @version 1.0
 * @date 2021-10-31
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {FALSE, TRUE} boolean;
typedef struct /* Struct que guarda um vetor de flips e seu tamanho */
{
    int* flipsVector;
    int amountOfFlips;
}flips;

boolean fillVector(int*, int);
void ordenaPanquecas(int *, int, flips*, int, int*);
void flip(int *, int, int);
boolean checaPilha(int *, int);

int main()
{
    int i, *solucao, /* vetor que guarda, na posição solucao[0], a quantidade
                        de flips e, ao longo do resto de seu corpo, 
                        os flips em questão */
        pancakeStackSize, *pancakeStack; /* a pilha de panquecas e seu tamanho */
    
    flips *flipCandidate; /* utilizado nas iterações e backtracking do algoritmo */

    printf("Insira a quantidade de panquecas a serem ordenadas: ");
    if(!scanf("%d", &pancakeStackSize))
    {
        printf("\nHouve um problema com a inserção da quantidade de panquecas.\n");
        return -1;
    }

    pancakeStack = (int *)malloc(pancakeStackSize*sizeof(int));

    flipCandidate = (flips *)malloc(sizeof(flips));
    flipCandidate->flipsVector = (int *)malloc((2*pancakeStackSize)*sizeof(int));
    flipCandidate->amountOfFlips = 0;
    
    solucao = (int *)malloc((1 + 2*pancakeStackSize)*sizeof(int));
    solucao[0] = 1 + 2*pancakeStackSize;

    printf("Insira a pilha de %d panquecas: ", pancakeStackSize);
    if(!fillVector(pancakeStack, pancakeStackSize))
    {
        printf("\nHouve um problema com a inserção da pilha de panquecas.\n");
        return -1;
    }

    ordenaPanquecas(pancakeStack, pancakeStackSize, flipCandidate, -1, solucao);
    
    if(solucao[0] > 0)
    {
        printf("\n");
        for(i = 1; i - 1 < solucao[0]; ++i)
            printf("%2d ", solucao[i]);
        printf("\n");
    }

    free(pancakeStack);
    free(flipCandidate->flipsVector);
    free(flipCandidate);
    free(solucao);
    return 0;
}

void ordenaPanquecas(int *stack, int stackSize, flips *candidatoAFlip, int posDoUltimoFlip, int *solucao)
{
    int i, j;

    if(candidatoAFlip->amountOfFlips < solucao[0]) /* Se a quantidade de flips atual for menor que o máximo estipulado */
    {
        if(!checaPilha(stack, stackSize)) /* Se a pilha de panquecas não estiver resolvida */
        {
            for(i = 0; i < stackSize; ++i) /* Para cada posição da pilha, inclusivea última */
                if(i != posDoUltimoFlip) /* Se a posição atual não foi a última a ser flipada */
                {
                    flip(stack, i, stackSize); /* Flipa a posição */
                    candidatoAFlip->flipsVector[(candidatoAFlip->amountOfFlips)++] = i; /* Contabiliza e armazena o flip */
                    
                    /* Recursão */
                    ordenaPanquecas(stack, stackSize, candidatoAFlip, candidatoAFlip->flipsVector[(candidatoAFlip->amountOfFlips) - 1], solucao);
                    
                    flip(stack, i, stackSize); /* Desflipa a posição */
                    (candidatoAFlip->amountOfFlips)--; /* Descontabiliza o flip */
                }  
        }
        else /* Se a pilha de panquecas estiver resolvida */
        {
            if(solucao[0] > candidatoAFlip->amountOfFlips) /* Se a quantidade de flips for menor que a da última solução */
            {
                solucao[0] = candidatoAFlip->amountOfFlips; /* Atualiza o máximo estipulado */
                /* Guarda os flips e a quantidade dos mesmos */
                for(j = 1; j <= candidatoAFlip->amountOfFlips; ++j)
                    solucao[j] = candidatoAFlip->flipsVector[j - 1];
            }
        }
    }
}

void flip(int *stack, int position, int end)
{
    int aux;

    if(position == end - 1)
        *(stack + position) *= -1;
    else
        while(position < end)
        {
            aux = (*(stack + position));
            *(stack + position++) = -(*(stack + end -1));
            *(stack + end-- -1) = - aux;
        }
}

boolean checaPilha(int * stack, int stackSize)
{
    for(; stackSize > 1; ++stack, --stackSize)
        if(abs(*(stack + 1)) > abs(*(stack)) || *(stack + 1) < 0 || *(stack) < 0)
            return FALSE;

    return TRUE;
}

boolean fillVector(int* vector, int size)
{
    for(; size > 0; ++vector, --size)
        if(!scanf("%d", vector))
            return FALSE;

    return TRUE;
}