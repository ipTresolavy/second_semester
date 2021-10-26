#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

void nRainhas(int n);
void printMatrix(int **matrix, int n);
int ** alocaMatrizQuadrada(int n);
void liberaMatriz(int ** matrix, int n);
int posicaoLivre(int **tab, int n, int lin, int col);


int main()
{
    int n;
    printf("Digite n > 0: ");
    if(scanf("%d", &n))
        nRainhas(n);
    return 0;
}

void nRainhas(int n)
{
    pilha *pos = criaPilha();
    int atual = 0, col = 0, **tab = alocaMatrizQuadrada(n), i, j, ok;

    for(i = 0; i < n; ++i)
        for(j = 0; j < n; ++j)
            tab[i][j] = 0;

    while(atual < n)
    {
        ok = 0;
        while(col < n && ok ==0)
        {
            if(posicaoLivre(tab, n, atual, col))
                ok = 1;
            else
                col++;
        }
        if(ok == 1)
        {
            empilha(pos, col);
            tab[atual][col] = 1;
            atual++;
            col = 0;
        }
        else
        {
            if(pilhaVazia(pos)){
                printf("Não há solução para n = %d\n", n);
                return;
            }
            col = desempilha(pos);
            atual--;
            tab[atual][col] = 0;
            col++;
        }
    }

    printMatrix(tab, n);
}

void printMatrix(int **matrix, int n)
{
    int i, j;

    for(i = 0UL; i < n; ++i)
        {
            for(j = 0; j < n; ++j)
                printf("%d ", matrix[i][j]);
            printf("\n");
        }
}

int** alocaMatrizQuadrada(int n)
{
    int **matrix, i;

    matrix = (int **)malloc(n*sizeof(int *));
        for(i = 0; i < n; ++i)
            matrix[i] = (int *)malloc(n*sizeof(int));

    return matrix;
}

void liberaMatriz(int **matrix, int n)
{
    int i;
    for(i = 0; i < n; ++i)
            free(matrix[i]);
        free(matrix);
}

int posicaoLivre(int **tab, int n, int lin, int col)
{
    int livre = 1, i, j;

    for(i = 0; i < n && livre == 1; ++i)
        if(tab[lin][i])
            livre = 0;
    
    for(i = 0; i < n && livre == 1; ++i)
        if(tab[i][col])
            livre = 0;

    for(i = 0; i < n && livre == 1; i++)
    {
        j = i + col - lin;
        if(j >= 0 && j < n && tab[i][j] == 1)
            livre = 0;
    }

    for(i = 0; i < n && livre == 1; i++)
    {
        j = col + lin - i;
        if(j >= 0 && j < n && tab[i][j] == 1)
            livre = 0;
    }
    return livre;
}