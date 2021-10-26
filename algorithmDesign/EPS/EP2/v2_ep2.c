#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

typedef enum {FALSE, TRUE}boolean;

int** createMatrix(int, int);
void printMatrix(int**, int, int);
boolean scanMatrix(int**, int, int, int, pilha*);
boolean fillMatrix(int**, int, int);


boolean restaUm(int**, int, int);
boolean movPermitido(int**, int, int, item*);
boolean checaTabuleiro(int**, int, int, pilha);
void backtrackTabuleiro(int**, item);
void checkMovs(pilha);
 



int main()
{
    int i, m, n, **tab;

    /* A título de curiosidade */
    /*printf("boolean type size: %lu\n", sizeof(boolean));*/

    printf("Insira a dimensão m x n do tabuleiro: ");
    if(!scanf("%d %d", &m, &n))
    {
        printf("\nHouve um erro com a inserção da dimensão do tabuleiro.\n");
        return 0;
    }

    tab = createMatrix(m, n);
    printf("Insira o tabuleiro:\n");
    if(!fillMatrix(tab, m, n))
    {
        printf("\nHouve um erro com a inserção de valores no tabuleiro.");
        return 0;
    }


    if(!restaUm(tab, m, n))
        printf("\nImpossível.\n");
    else
        printMatrix(tab, m, n);


    for(i = 0; i < m; ++i)
        free(tab[i]);
    free(tab);

    return 0;
}

boolean restaUm(int** tab, int m, int n)
{
    pilha *movs = criaPilha(), *buracos = criaPilha(), *aux = criaPilha();
    item pcAMover;
    boolean achou = FALSE;

    if(!scanMatrix(tab, m, n, -1, buracos))
    {
        destroiPilha(movs);
        destroiPilha(aux);
        destroiPilha(buracos);
        return FALSE;
    }
    printf("\nBuraco: %d:%d\n", topoDaPilha(buracos).l, topoDaPilha(buracos).c);

    pcAMover.l = pcAMover.c = pcAMover.cDest = pcAMover.lDest = pcAMover.dir = 0;
    /*Empilha movimento inválido para controle do loop a seguir*/
    /*empilha(movs, pcAMover);*/

    /*while(!(pilhaVazia(movs) && pcAMover.dir == 4))*/
    while(!achou)
    {
        if(pcAMover.l < m && pcAMover.c < n && tab[pcAMover.l][pcAMover.c] == 1 && movPermitido(tab, m, n, &pcAMover))
        {
            /*if(movs->topo >= 27)
            {
                printf("movs:\n");
                checkMovs(*movs);
            }*/
            empilha(movs, pcAMover);
            /*printf("%d:%d-%d:%d\n", (topoDaPilha(movs)).l, (topoDaPilha(movs)).c, (topoDaPilha(movs)).lDest, (topoDaPilha(movs)).cDest );
            */pcAMover.l = pcAMover.c = pcAMover.cDest = pcAMover.lDest = pcAMover.dir = 0;
        }
        else if(pcAMover.c < n)
        {
            ++(pcAMover.c);
            pcAMover.dir = 0;
        }
        else if (pcAMover.l < m)
        {
            ++(pcAMover.l);
            pcAMover.c = 0;
            pcAMover.dir = 0;
        }
        else if(!checaTabuleiro(tab, m, n, *buracos))
        {
            if(!pilhaVazia(movs))
                pcAMover = desempilha(movs);
            else
            {
                destroiPilha(movs);
                destroiPilha(aux);
                destroiPilha(buracos);
                return FALSE;
            }
            
            backtrackTabuleiro(tab, pcAMover);
            pcAMover.lDest = pcAMover.cDest = 0;
            ++(pcAMover.dir);
        }
        else
        {
            while (!pilhaVazia(movs))
                empilha(aux, desempilha(movs));

            /*desempilha(aux);*/
            
            while(!pilhaVazia(aux))
            {
                printf("%d:%d-%d:%d\n", (topoDaPilha(aux)).l, (topoDaPilha(aux)).c, (topoDaPilha(aux)).lDest, (topoDaPilha(aux)).cDest );
                desempilha(aux);
            }

            achou = TRUE;
            /*
            destroiPilha(movs);
            destroiPilha(aux);
            destroiPilha(buracos);
            return TRUE;*/
        }
    }

    destroiPilha(movs);
    destroiPilha(aux);
    destroiPilha(buracos);
    return TRUE;
}

void checkMovs(pilha bruh)
{
    while(!pilhaVazia(&bruh))
    {
        printf("%d:%d-%d:%d\n", (topoDaPilha(&bruh)).l, (topoDaPilha(&bruh)).c, (topoDaPilha(&bruh)).lDest, (topoDaPilha(&bruh)).cDest );
        desempilha(&bruh);
    }
    
}

boolean scanMatrix(int** tab, int m, int n, int value, pilha *p)
{
    item it;
    int i, j;
    boolean achou = FALSE;

    for(i = 0; i < m; ++i)
        for(j = 0; j < n; ++j)
            if(tab[i][j] == value)
            {
                it.l = i;
                it.c = j;
                empilha(p, it);
                achou = TRUE;
            }

    return achou;
}

boolean movPermitido(int** tab, int m, int n, item* peca)
{
    while(peca->dir < 4)
        switch (peca->dir)
        {
        case 0:
            if(peca->c + 2 < n && tab[peca->l][peca->c + 1] == 1 && tab[peca->l][peca->c + 2] == -1)
            {
                peca->lDest = peca->l;
                peca->cDest = peca->c + 2;

                tab[peca->l][peca->c] = -1;
                tab[peca->l][peca->c + 1] = -1;
                tab[peca->lDest][peca->cDest] = 1;
                return TRUE;
            }
            else
                ++(peca->dir);
            break;
        case 1:
            if(peca->l - 2 >= 0 && tab[peca->l - 1][peca->c] == 1 && tab[peca->l - 2][peca->c] == -1)
            {
                peca->lDest = peca->l - 2;
                peca->cDest = peca->c;

                tab[peca->l][peca->c] = -1;
                tab[peca->l - 1][peca->c] = -1;
                tab[peca->lDest][peca->cDest] = 1;
                return TRUE;
            }
            else
                ++(peca->dir);
            break;
        case 2:
            if(peca->c - 2 >= 0 && tab[peca->l][peca->c - 1] == 1 && tab[peca->l][peca->c - 2] == -1)
            {
                peca->lDest = peca->l;
                peca->cDest = peca->c - 2;

                tab[peca->l][peca->c] = -1;
                tab[peca->l][peca->c - 1] = -1;
                tab[peca->lDest][peca->cDest] = 1;
                return TRUE;
            }
            else
                ++(peca->dir);
            break;
        case 3:
            if(peca->l + 2 < m && tab[peca->l + 1][peca->c] == 1 && tab[peca->l + 2][peca->c] == -1)
            {
                peca->lDest = peca->l + 2;
                peca->cDest = peca->c;

                tab[peca->l][peca->c] = -1;
                tab[peca->l + 1][peca->c] = -1;
                tab[peca->lDest][peca->cDest] = 1;
                return TRUE;
            }
            else
                ++(peca->dir);
            break;
        }

    return FALSE;
}

boolean checaTabuleiro(int** tab, int m, int n, pilha buracos)
{
    int i, j;

    for(i = m - 1; i >= 0; --i)
        for(j = n - 1; j >= 0; --j)
            if(!pilhaVazia(&buracos) && i == topoDaPilha(&buracos).l && j == topoDaPilha(&buracos).c)
                if(tab[i][j] != 1)
                    return FALSE;
                else
                    desempilha(&buracos);
            else if(tab[i][j] == 1)
                return FALSE;

    return TRUE;
}

void backtrackTabuleiro(int** tab, item peca)
{
    tab[peca.lDest][peca.cDest] = -1;
    tab[peca.l][peca.c] = 1;

    switch (peca.dir)
    {
    case 0:
        tab[peca.l][peca.c + 1] = 1;
        break;
    
    case 1:
        tab[peca.l - 1][peca.c] = 1;
        break;

    case 2:
        tab[peca.l][peca.c - 1] = 1;
        break;

    case 3:
        tab[peca.l + 1][peca.c] = 1;
        break;
    }

}

int** createMatrix(int m, int n)
{
    int **matrix, i;

    matrix = (int **)malloc(m*sizeof(int *));
        for(i = 0; i < m; ++i)
            matrix[i] = (int *)malloc(n*sizeof(int));

    return matrix;
}

boolean fillMatrix(int** matrix, int m, int n)
{
    int i, j;

    for(i = 0; i < m; ++i)
        for(j = 0; j < n; ++j)
            if(!scanf("%d", &matrix[i][j]))
                return FALSE;

    return TRUE;
}

void printMatrix(int **matrix, int m, int n)
{
    int i, j;

    for(i = 0; i < m; ++i)
    {
        for(j = 0; j < n; ++j)
            printf("%2d ", matrix[i][j]);
        printf("\n");
    }
}

