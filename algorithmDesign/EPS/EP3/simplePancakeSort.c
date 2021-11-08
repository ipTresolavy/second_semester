/**
 * @file simplePancakeSort.c
 * @author Igor Pontes Tresolavy (tresolavy@usp.br)
 * @brief Segundo EP da disciplina de MAC0122 (Pancake sort)
 * @version 1.0
 * @date 2021-10-28
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE, TRUE} boolean;

boolean fillVector(int*, int);
void printVector(int*, int);
void ordenaPanquecas(int *, int);
int elementoMax(int *, int, int);
void flip(int *, int, int);

int main()
{
    int pancakeStackSize, *pancakeStack;

    printf("Insira a quantidade de panquecas a serem ordenadas: ");
    if(!scanf("%d", &pancakeStackSize))
    {
        printf("\nHouve um problema com a inserção da quantidade de panquecas.\n");
        return -1;
    }

    pancakeStack = (int *)malloc(pancakeStackSize*sizeof(int));

    printf("Insira a pilha de %d panquecas: ", pancakeStackSize);
    if(!fillVector(pancakeStack, pancakeStackSize))
    {
        printf("\nHouve um problema com a inserção da pilha de panquecas.\n");
        return -1;
    }

    ordenaPanquecas(pancakeStack, pancakeStackSize);
    printVector(pancakeStack, pancakeStackSize);

    return 0;
}

void ordenaPanquecas(int *stack, int stackSize)
{
    int i, maxElemInd;

    printf("\n");
    for(i = 0; i < stackSize - 1; ++i)
    {
        maxElemInd = elementoMax(stack, i, stackSize);

        if(maxElemInd != i)
        {
            if(maxElemInd == stackSize - 1)
            {
                flip(stack, i, stackSize);
                printf("flip(%d): ", i);
                printVector(stack, stackSize);
                /* printf("%2d ", i); */
            }
            else
            {
                flip(stack, maxElemInd, stackSize);
                printf("flip(%d): ", maxElemInd);
                printVector(stack, stackSize);
                /* printf("%2d ", maxElemInd); */

                flip(stack, i, stackSize);
                printf("flip(%d): ", i);
                printVector(stack, stackSize);
                /* printf("%2d ", i); */
            }
        }
    }
    printf("\n");
}

void flip(int *stack, int position, int end)
{
    int aux;

    while(position < end)
    {
        aux = *(stack + position);
        *(stack + position++) = *(stack + end -1);
        *(stack + end-- -1) = aux;
    }
}

int elementoMax(int *vector, int begining, int end)
{
    int maxInd, i;
    for (maxInd = end - 1, i = end - 1; i >= begining; --i)
        if (vector[i] > vector[maxInd])
            maxInd = i;
    return maxInd;
}

boolean fillVector(int* vector, int size)
{
    for(; size > 0; ++vector, --size)
        if(!scanf("%d", vector))
            return FALSE;

    return TRUE;
}

void printVector(int* vector, int size)
{
    for(; size > 0; ++vector, --size)
        printf("%2d ", *vector);
    printf("\n");
}