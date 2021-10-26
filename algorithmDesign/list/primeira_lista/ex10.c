#include <stdio.h>
#include <stdlib.h>
#include "pilha10.h" /* pilha com item tipo int */

#define MAX_CORES 4

typedef enum {FALSE, TRUE}boolean;

int** createSquareMatrix(int);
boolean fillMatrix(int**, int);
boolean quatroCores (int **, int, int *);
char* decoderDeCores(char);

int main()
{
    int **mapa, *cores;
    int n, i;


    printf("Insira o tamanho \"n\" do mapa: ");
    if(!scanf("%d", &n))
    {
        printf("Houve um problema com o valor inserido.\n");
        return 0;
    }


    mapa = createSquareMatrix(n);
    cores = (int *)malloc(n*sizeof(int));


    printf("Insira a matriz do mapa:\n");
    if(!fillMatrix(mapa, n))
    {
        printf("Houve um problema com o mapa inserido.\n");
        return 0;
    }


    if(quatroCores(mapa, n, cores))
    {
        printf("\nPossível:\n");
        
        for(i = 0; i < n; ++i)
            printf("País %d: %s\n", i,decoderDeCores(cores[i]));

        printf("\n");
    }
    else
        printf("\nImpossível\n\n");

    
    for(i = 0; i < n; ++i)
        free(mapa[i]);
    free(mapa);

    free(cores);


    return 0;
}

boolean quatroCores (int ** mapa, int n, int * cores)
{
    pilha *pilhaCores = criaPilha();
    int i;

    /* enquanto todos os países não receberem cores VÁLIDAS */
    while(pilhaCores->topo < n)
    {
        /* inicializa cor do país com 0 */
        empilha(pilhaCores, 0);
        
        /* enquanto todos os vizinhos não tiverem sido verificados */
        for(i = 0; i < pilhaCores->topo;)
            /* evita a comparação de um país com ele mesmo, verifica se o país em análise é vizinho 
            do país que está sendo colorido e verifica se sua cor é a mesma do país em análise*/
            if(i != (pilhaCores->topo) - 1 && mapa[i][(pilhaCores->topo) - 1] == 1 && pilhaCores->v[i] == topoDaPilha(pilhaCores))
                /* caso a cor seja igual, se nem todas as cores tiverem sido testadas, testa outra cor */
                if(MAX_CORES > topoDaPilha(pilhaCores) + 1)
                {
                    empilha(pilhaCores, desempilha(pilhaCores) + 1);
                    i = 0;
                }
                else
                {
                    /*Se todas as cores tiverem sido testadas, tenta mudar a cor do país anterior  
                    e repete o mesmo teste*/
                    while(!pilhaVazia(pilhaCores) && MAX_CORES <= topoDaPilha(pilhaCores) + 1)
                        desempilha(pilhaCores);

                    /*Se todas as cores para todos os países até então tiverem sido testadas
                    termina o programa*/
                    if(pilhaVazia(pilhaCores))
                        return FALSE;
                    /* atualiza cor de um país anterior e procede para verificar
                    seus vizinhos novamente */
                    else
                        empilha(pilhaCores, desempilha(pilhaCores) + 1);

                    i = 0;
                }
            else
                ++i;
    }   

    for(i = 0; i < (pilhaCores->topo); ++i)
        cores[i] = pilhaCores->v[i];


    destroiPilha(pilhaCores);
    return TRUE;
}

char* decoderDeCores(char c)
{
    switch (c)
    {
    case 0:
        return "Verde";
        break;

    case 1:
        return "Amarelo";
        break;

    case 2:
        return "Azul";
        break;

    case 3:
        return "Vermelho";
        break;

    default:
        /* Nunca deverá chegar nessa condição */
        return "Mais de 4 cores no mapa";
        break;
    }
}

int** createSquareMatrix(int n)
{
    int **matrix, i;

    matrix = (int **)malloc(n*sizeof(int *));
        for(i = 0; i < n; ++i)
            matrix[i] = (int *)malloc(n*sizeof(int));

    return matrix;
}

boolean fillMatrix(int** matrix, int size)
{
    int i, j;

    for(i = 0; i < size; ++i)
        for(j = 0; j < size; ++j)
            if(!scanf("%d", &matrix[i][j]))
                return FALSE;

    return TRUE;
}
