/**
 * @file ex05.c
 * @author Igor Pontes Tresolavy (tresolavy@usp.br)
 * @brief 
 * @version 1.0
 * @date 2021-11-06
 */
#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE, TRUE}boolean;

boolean fillVector(int*,int);
void printVector(int*,int);
int scanVector(int*, int, int, int);
void ordenaVetor(int*,int);

int main()
{
    int  n, *V;

    printf("Digite o tamanho do vetor: ");
    if(!scanf("%d", &n))
    {
        printf("Houve um erro com a inserção do tamanho do vetor.");
        return -1;
    }

    V = (int *)malloc(n*sizeof(int));
    printf("Insira o vetor: ");
    if(!fillVector(V, n))
    {
        printf("Houve um erro com a inserção do vetor.");
        return -1;
    }

    ordenaVetor(V, n);
    printVector(V, n);

    free(V);
    return 0;
}

void ordenaVetor(int* V, int n)
{
    int i, j, *cont = (int*)malloc(n*sizeof(int));
    int *aux = (int*)malloc(n*sizeof(int));

    for (i = 0; i < n; i++) 
        cont[i] = 0;
    
    for (i=0; i < n; i++)
        for (j = 0; j < n; j++)
            if (V[j] < V[i]) 
                cont[i]++;

    /* Opcional, caso queira lidar com elementos repetidos */
    for(i = 0; i < n; ++i)
        if((j = scanVector(cont, n, i, cont[i])) > 1)
            cont[i] += j - 1;

    for (i=0; i < n; ++i)
        if(i != cont[i])
            aux[cont[i]] = V[i];

    for (i = 0; i < n; ++i)
        if(i != cont[i])
            V[i] = aux[i];

    free(aux);
    free(cont);
}

boolean fillVector(int* vector, int size)
{
    for(; size > 0; --size)
        if(!scanf("%d", vector++))
            return FALSE;

    return TRUE;
}

int scanVector(int* vector, int n, int start ,int value)
{
    int i;
    int qntdValue = 0;

    for(i = start; i < n; ++i)
        if(vector[i] == value)
            ++qntdValue;

    return qntdValue;
}

void printVector(int* vector, int size)
{
    for(; size > 0; --size)
        printf("%d ", *vector++);
    printf("\n");
}