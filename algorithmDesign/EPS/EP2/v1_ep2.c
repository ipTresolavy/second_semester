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

int main()
{
    int i, m, n, **tab;

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
        printf("\nImpossivel\n");
    /*else
        printMatrix(tab, m, n);*/


    for(i = 0; i < m; ++i)
        free(tab[i]);
    free(tab);

    return 0;
}

boolean restaUm(int** tab, int m, int n)
{
    /* *movs guarda os movimentos e *buracos, todos os buracos. *aux é usada para imprimir *mov
    na ordem certa.*/
    pilha *movs = criaPilha(), *buracos = criaPilha(), *aux = criaPilha();
    item pcAMover;
    boolean achou = FALSE, possivel = TRUE;

    /* procura todos os buracos no tabuleiro */
    if(!scanMatrix(tab, m, n, -1, buracos))
        possivel = FALSE;/* caso não haja nenhum, o tabuleiro é impossível */

    /* inicializa a peça que vai tentar ser movida. */
    pcAMover.l = pcAMover.c = pcAMover.cDest = pcAMover.lDest = pcAMover.dir = 0;

    /* enquanto nenhuma solução for achada ou todas as possibilidades forem testadas */
    while(!achou && possivel)
    {
        /* checa, no sentido anti-horário, se há movimento permitido em alguma direção*/
        if(pcAMover.l < m && pcAMover.c < n && tab[pcAMover.l][pcAMover.c] == 1 && movPermitido(tab, m, n, &pcAMover))
        {
            /* caso haja, executa o movimento e volta para o início do tabuleiro */
            empilha(movs, pcAMover);
            pcAMover.l = pcAMover.c = pcAMover.cDest = pcAMover.lDest = pcAMover.dir = 0;
        }
        else if(pcAMover.c < n)/* caso contrário, tenta mover próxima peça à direita */
        {
            ++(pcAMover.c);
            pcAMover.dir = 0;
        }
        else if (pcAMover.l < m)/* caso contrário, tenta mover a primeira peça na linha abaixo*/
        {
            ++(pcAMover.l);
            pcAMover.c = 0;
            pcAMover.dir = 0;
        }
        else if(!checaTabuleiro(tab, m, n, *buracos))/* caso nenhum movimento a mais seja permitido, checa se encontrou solução */
        {
            if(!pilhaVazia(movs))/* se a pilha estiver vazia, todos os movimentos já foram testados */
            {
                pcAMover = desempilha(movs);
                backtrackTabuleiro(tab, pcAMover);
                pcAMover.lDest = pcAMover.cDest = 0;
                ++(pcAMover.dir);/* se não estiver, volta no último movimento, restaura o tabuleiro e testa a próxima direção */
            }
            else
                possivel = FALSE;
        }
        else/* caso tenha encontrado uma solução */
        {
            while (!pilhaVazia(movs))
                empilha(aux, desempilha(movs));/* inverte a pilha *movs na pilha *aux */
            
            while(!pilhaVazia(aux))/* imprime a pilha *aux */
            {
                printf("%d:%d-%d:%d\n", (topoDaPilha(aux)).l, (topoDaPilha(aux)).c, (topoDaPilha(aux)).lDest, (topoDaPilha(aux)).cDest );
                desempilha(aux);
            }

            achou = TRUE;
        }
    }

    destroiPilha(movs);
    destroiPilha(aux);
    destroiPilha(buracos);
    return (achou && possivel)?(TRUE):(FALSE);/* se tiver encontrado uma solução possível, retorna verdadeiro */
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
    /* "item *peca" não é uma pilha, e sim um struct. Portanto, é permitido acessar seus valores da forma abaixo*/
    while(peca->dir < 4)
        switch (peca->dir)
        {
        case 3:/* direita */
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
        case 0:/* cima */
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
        case 1:/* esquerda */
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
        case 2:/* baixo */
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

void backtrackTabuleiro(int** tab, item peca)/* retorna as peças que foram retiradas anteriormente */
{
    tab[peca.lDest][peca.cDest] = -1;
    tab[peca.l][peca.c] = 1;

    switch (peca.dir)
    {
    case 3:
        tab[peca.l][peca.c + 1] = 1;
        break;
    
    case 0:
        tab[peca.l - 1][peca.c] = 1;
        break;

    case 1:
        tab[peca.l][peca.c - 1] = 1;
        break;

    case 2:
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

