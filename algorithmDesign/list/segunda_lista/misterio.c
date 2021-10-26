#include <stdio.h>
#include <stdlib.h>

int misterio (int A[], int inic, int fim);

int main()
{
    int i, comp, A[] = {8, 10, 3, 6, 5, 2, 9, 1, 4};
    int inic = 0;
    int fim = sizeof(A)/sizeof(A[0]);
    
    for (i = 0; i < fim; i++)
        printf(" %d ", A[i]);
    printf("\n");

    comp = misterio(A, inic, fim - 1);
    for (i = 0; i < fim; i++)
        printf(" %d ", A[i]);
    printf("\n");
    printf("\nComparações: %d\n", comp);
    
    
    return 0;
}

int misterio (int A[], int inic, int fim)
{
    int aux, i;
    static int comp;

    while (A[fim] % 2 == 0.0 && inic < fim)
    {
        fim--;
        comp++;
    }
    comp++;/* Contabiliza comparação que falha nos requerimentos do 'while'*/

    while (A[inic] % 2 == 1 && inic < fim)
    {
        inic++;
        comp++;
    }
    comp++;/* Contabiliza comparação que falha nos requerimentos do 'while'*/

    if (inic < fim)
    {
        aux = A[inic];
        A[inic] = A[fim];
        A[fim] = aux;
        fim--;
        inic++;/*Evita que valores que acabaram de ser comparados sejam comparados novamente*/
        for (i = 0; i < 9; i++)
            printf(" %d ", A[i]);
        printf("\n");        
        misterio(A, inic, fim);
    }
    return comp;
}