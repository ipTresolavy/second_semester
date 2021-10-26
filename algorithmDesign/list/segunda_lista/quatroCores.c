#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE, TRUE}boolean;

int** createSquareMatrix(int);
boolean fillMatrix(int**, int);
boolean fillVector(int* vector, int size);
boolean quatrocores(int **mapa, int n, int *cores, int atual);
char* decoderDeCores(char c);


int main()
{
    int **mapa, *cores;
    int n, i, atual = 0;


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

    printf("Insira o vetor de cores do mapa:\n");
    if(!fillVector(cores, n))
    {
        printf("Houve um problema com o mapa inserido.\n");
        return 0;
    }

    if(quatrocores(mapa, n, cores, atual))
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

boolean quatrocores(int **mapa, int n, int *cores, int atual)
{
    int i, j;

    if(atual < n) /* Se todos os países ainda não tiverem sido testados*/
    {
        for (i = 0; i < 4; ++i) /* Para cada cor */
        {
            cores[atual] = i; /* colore o país */
    
            if (quatrocores(mapa, n, cores, atual + 1)) /* chama recursivamente o próximo país a ser colorido */
                return TRUE;/* Se bem sucedido, retorna verdadeiro */
    
            cores[atual] = 0;/* reinicia cor se não for bem sucedido */
        }
    }
    else
    {
        for(i = 0; i < n; ++i)
            for(j = i + 1; j < n; ++j)
                if(mapa[i][j] && cores[j] == cores[i])
                    return FALSE;/* retorna falso se, após colorir todos os países
                                    ainda houver países vizinhos com cores iguais */

        return TRUE;/* retorna verdadeiro, caso contrário */
    }

    return FALSE; /* Se não for possível colorir os países, retorna falso */
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

boolean fillVector(int* vector, int size)
{
    int i;

    for(i = 0; i < size; ++i)
        if(!scanf("%d", &vector[i]))
            return FALSE;

    return TRUE;
}