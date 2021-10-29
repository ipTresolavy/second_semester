/**
 * @file ep2.c
 * @author Igor Pontes Tresolavy (tresolavy@usp.br)
 * @brief Segundo EP da disciplina de MAC0122
 * @version 1.0
 * @date 2021-10-28
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pilha.h"

/* 

Uma breve explicação sobre as funções pagTab, pagVal, pagValFinal e tabClass:

As funções pag se referem à Pagoda Function ou Função Pagode. Elas se utilizam 
de uma relação presente nos tabuleiros do tipo de problema de Resta Um do EP 
para tomar conclusões acerca de quais movimentos são permitidos e quais não são.

De maneira simplificada, o método consiste em atribuir valores para cada célula
do tabuleiro de maneira que a soma de dois valores em qualquer trio de células
adjacentes será maior ou igual ao valor da célula seguinte, em qualquer direção.
O Valor Pagoda de um tabuleiro consiste na soma dos valores de todas as células
ocupadas por uma peça. Uma consequência do método é o fato de que o Valor Pagoda
em qualquer estado do tabuleiro deve ser sempre maior ou igual ao valor pagoda
do tabuleiro em seu estado final, ou seja, na solução. Essa relação é usada para
impedir movimentos que diminuam o valor pagoda do tabuleiro para menos do que o
valor final.

A função tabClass, por sua vez, divide as diagonais do tabuleiro em 6 grupos, 3
para as diagonais descendentes e 3 para as ascendentes. 

A paridade da quantidade T de peças total no tabuleiro menos a quantidade de 
peças Ni em cada diagonal, para i entre 0 e 5, deve permanecer a mesma entre
todos os estados do tabuleiro. Portanto se o conjunto de T - Ni para um tabuleiro
incial é {Par, Ímpar, Par, Par, Ímpar, Ímpar}, por exemplo, o do estado final
deve ser igual para que o problema seja resolvível. Caso contrário, é impossível
resolver o tabuleiro proposto e o programa termina.

Fontes:
http://www.gibell.net/pegsolitaire/#nullclass
https://www.cs.york.ac.uk/aig/projects/implied/docs/CPAIOR03.pdf
https://www.link.cs.cmu.edu/15859-s11/notes/peg-solitaire.pdf

*/

typedef enum {FALSE, TRUE} boolean;

int** createMatrix(int, int);
void printMatrix(int**, int, int);
int scanMatrix(int**, int, int, int, pilha*);/* devolve a quantidade de certo elemento em uma matriz*/
boolean fillMatrix(int**, int, int);

boolean restaUm(int**, int, int);/* tenta resolver o problema pedido */
boolean movPermitido(int**, int, int, item*);/* procura um movimento permitido e, se houver, o executa */
boolean checaTabuleiro(int**, int, int, pilha);/* checa se encontrou uma solução */
void backtrackTabuleiro(int**, item);/* restaura o tabuleiro para a condição anterior a um movimento */
void pagTab(int**, long double**, int, int, item);/* cria e atribui valores pagoda à cada célula do tabuleiro */
long double pagVal(int**, long double**, int, int);/* devolve o valor pagoda do tabuleiro no estado atual */
long double pagValFinal(long double**, pilha);/* devolve o valor pagoda no estado final, ou seja, na solução */
boolean tabClass(int**, int, int, int, int);/* checa se o estado incial e final são da mesma classe */

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

    for(i = 0; i < m; ++i)
        free(tab[i]);
    free(tab);

    return 0;
}

boolean restaUm(int** tab, int m, int n)
{
    pilha *movs = criaPilha(), /* *movs guarda os movimentos */
        *buracos = criaPilha(), /* *buracos guarda os buracos */
        *pilhaAux = criaPilha();/* *aux é usada para imprimir *mov na ordem certa e 
                                para contar a quantidade de 1's no tabuleiro. */

    item pcAMover;/* peça candidata a ser movida */

    boolean achou = FALSE,/* flags de estado do tabuleiro */
        possivel = TRUE;

    int i, estado, /* usado para delimitar a posição no vetor *pagodaValue */
        qntdBuracos,
        qntdEstados;/* quantidade de estados entre tabuleiro incial e final */

    long double *pagodaValue, /* vetor de valores pagoda para cada estado do tabuleiro */
        **pagodaTab, /* tabuleiro com valores pagoda para cada célula */
        finalPagodaValue;/* valor pagoda final */

    /* procura todos os buracos no tabuleiro */
    if(!(qntdBuracos = scanMatrix(tab, m, n, -1, buracos)))
        possivel = FALSE;/* caso não haja nenhum, o tabuleiro é impossível */

    /* 
    *pagodaValue guarda o valor pagoda do tabuleiro para cada estado possível. Há
    um estado para cada movimento (scanMatrix(tab, m, n, 1, pilhaAux) - qntdBuracos)
    mais o estado incial (+1).    
    */
    pagodaValue = (long double *)malloc((qntdEstados = scanMatrix(tab, m, n, 1, pilhaAux) - qntdBuracos + 1)*sizeof(long double));
    pagodaTab = (long double **)malloc(m*sizeof(long double *));
    for(i = 0; i < m; ++i)
        pagodaTab[i] = (long double *)malloc(n*sizeof(long double));
    
    /* preenche *pagodaTab com base no último buraco encontrado*/
    if(!pilhaVazia(buracos))
        pagTab(tab, pagodaTab, m, n, topoDaPilha(buracos));
    else
        possivel = FALSE;

    /* verifica se o tabuleiro incial e final são da mesma classe */
    if(!tabClass(tab, m, n, qntdEstados + qntdBuracos - 1, qntdBuracos))
        possivel = FALSE;

    /* 
    inicializa *pagodaValue com o valor pagoda do estado inicial 
    e inicializa finalpagodaValue com o valor pagoda final
    */
    estado = 0;
    pagodaValue[estado] = pagVal(tab, pagodaTab, m, n);
    ++estado;
    finalPagodaValue = pagValFinal(pagodaTab, *buracos);

    /* inicializa a peça candidata a ser movida. */
    pcAMover.l = pcAMover.c = pcAMover.cDest = pcAMover.lDest = pcAMover.dir = 0;
    
    /* enquanto nenhuma solução for encontrada ou todas as possibilidades forem testadas */
    while(!achou && possivel)
    {
        /* checa se há estados faltantes e, no sentido anti-horário, se há movimento permitido em alguma direção*/
        if(estado < qntdEstados && pcAMover.l < m && pcAMover.c < n && tab[pcAMover.l][pcAMover.c] == 1 && movPermitido(tab, m, n, &pcAMover))
        {
            /* se o valor pagoda após o movimento for maior ou igual ao valor pagoda final */
            if((pagodaValue[estado] = pagVal(tab, pagodaTab, m, n)) >= finalPagodaValue)
            {
                /* o movimento é válido */
                empilha(movs, pcAMover);
                pcAMover.l = pcAMover.c = pcAMover.cDest = pcAMover.lDest = pcAMover.dir = 0;
                estado++;
            }
            else
            {
                /* caso contrário, o movimento inválido é desfeito */
                backtrackTabuleiro(tab, pcAMover);
                pcAMover.lDest = pcAMover.cDest = 0;
                ++(pcAMover.dir);/* testa a próxima direção */
                pagodaValue[estado] = 0;
            }
        }
        /* caso ainda haja estados faltantes e a peça não possa ser movida */
        else if(estado < qntdEstados && pcAMover.c < n)
        {
            /* seleciona próxima peça à direita, se existir */
            ++(pcAMover.c);
            pcAMover.dir = 0;
        }
        else if (estado < qntdEstados && pcAMover.l < m)
        {
            /*ou seleciona primeira peça na linha abaixo, se existir */
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
                --estado;
            }
            else
                possivel = FALSE;
        }
        else/* caso tenha encontrado uma solução */
        {
            while (!pilhaVazia(pilhaAux))
                desempilha(pilhaAux);/* desempilha "sujeira" anterior do programa*/

            while (!pilhaVazia(movs))
                empilha(pilhaAux, desempilha(movs));/* inverte a pilha *movs na pilha *pilhaAux */
            
            while(!pilhaVazia(pilhaAux))/* imprime a pilha *pilhaAux */
            {
                printf("%d:%d-%d:%d\n", (topoDaPilha(pilhaAux)).l, (topoDaPilha(pilhaAux)).c, (topoDaPilha(pilhaAux)).lDest, (topoDaPilha(pilhaAux)).cDest );
                desempilha(pilhaAux);
            }

            achou = TRUE;
        }
    }

    destroiPilha(movs);
    destroiPilha(pilhaAux);
    destroiPilha(buracos);
    for(i = 0; i < m; ++i)
            free(pagodaTab[i]);
    free(pagodaTab);
    free(pagodaValue);
    return (achou && possivel)?(TRUE):(FALSE);/* se tiver encontrado uma solução possível, retorna verdadeiro */
}

void pagTab(int** tab, long double** pagodaTab, int m, int n, item buraco)
{
    const long double x = (sqrt(5) - 1)/2;/* (razão áurea) - 1 */
    int i, j;

    for(i = 0; i < m; ++i )        
        for(j = 0; j < n; ++j)
            if(tab[i][j] != 0)
                pagodaTab[i][j] = pow(x, abs(buraco.c - j) + abs(buraco.l - i));
                /* x elevado à Distância Manhattan da célula tab[i][j] até o buraco*/
            else
                pagodaTab[i][j] = 0;    
}

long double pagVal(int** tab, long double** pagodaTab, int m, int n)
{
    int i, j;
    long double pagodaValue;

    for(i = 0, pagodaValue = 0.0F; i < m; ++i)        
        for(j = 0; j < n; ++j)
            if(tab[i][j] > 0)
                pagodaValue += pagodaTab[i][j];
                /* soma todos os valores pagoda de células ocupadas por peças */

    return pagodaValue;
}

long double pagValFinal(long double** pagodaTab, pilha buracos)
{
    long double finalPagodaValue = 0.0F;

    while(!pilhaVazia(&buracos))
    {
        finalPagodaValue += pagodaTab[topoDaPilha(&buracos).l][topoDaPilha(&buracos).c];
        desempilha(&buracos);
        /* soma todos os valores pagoda de células vazias */
    }

    return finalPagodaValue;
}

boolean tabClass(int** tab, int m, int n, int qntdPecas, int qntdPecasFinal)
{
    /* 
    valor máximo de uma subtração entre as coordenadas i e j de uma
    matrix m x n sempre será igual a max{m, n}
    */
    int i, j, maxSub = (m > n)?(m):(n), 
        startDiagonalClasses[] = {0, 0, 0, 0, 0, 0},/* guarda os valores iniciais das 6 classes */ 
        endDiagonalClasses[] = {0, 0, 0, 0, 0, 0};/* guarda os valores finais das 6 classes */

    for(i = 0; i < m; ++i)
        for(j = 0; j < n; ++j)
            if (tab[i][j] == 1)
            {
                startDiagonalClasses[(i + j)%3] += 1;
                startDiagonalClasses[3 + (maxSub + (i - j))%3] += 1;
            }
            else if (tab[i][j] == -1)
            {
                endDiagonalClasses[(i + j)%3] += 1;
                endDiagonalClasses[3 + (maxSub + (i - j))%3] += 1;
            }
        
    for(i = 0; i < 6; ++i)
    {
        startDiagonalClasses[i] = (qntdPecas - startDiagonalClasses[i])%2;
        endDiagonalClasses[i] = (qntdPecasFinal - endDiagonalClasses[i])%2;
        if(startDiagonalClasses[i] != endDiagonalClasses[i])
            return FALSE;
    }

    return TRUE;
}

boolean movPermitido(int** tab, int m, int n, item* peca)
{
    /* 
    "item *peca" não é uma pilha, e sim um struct. 
    Portanto, acessa-se seus valores da maneira abaixo
    */
    while(peca->dir < 4)
        switch (peca->dir)
        {
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
        }

    return FALSE;
}

boolean checaTabuleiro(int** tab, int m, int n, pilha buracos)
{
    int i, j;

    /* 
    como os buracos estão ordenados de (0, 0) até (m - 1, n - 1),
    seus valores podem ser verificados de (m - 1, n - 1) até (0, 0)
    */
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
    /* 
    recoloca as peças que foram retiradas pelo último movimento 
    e libera o espaço ocupado no mesmo movimento
    */
    tab[peca.lDest][peca.cDest] = -1;
    tab[peca.l][peca.c] = 1;

    switch (peca.dir)
    {    
    case 0:/* volta de cima */
        tab[peca.l - 1][peca.c] = 1;
        break;

    case 1:/* volta da esquerda */
        tab[peca.l][peca.c - 1] = 1;
        break;

    case 2:/* volta de baixo */
        tab[peca.l + 1][peca.c] = 1;
        break;
    case 3:/* volta da direita*/
        tab[peca.l][peca.c + 1] = 1;
        break;
    }

}

int scanMatrix(int** tab, int m, int n, int value, pilha *p)
{
    item it;
    int i, j;
    int qntdValue = 0;

    for(i = 0; i < m; ++i)
        for(j = 0; j < n; ++j)
            if(tab[i][j] == value)
            {
                it.l = i;
                it.c = j;
                empilha(p, it);
                ++qntdValue;
            }

    return qntdValue;
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