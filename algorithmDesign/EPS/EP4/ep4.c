/**
 * @file ep4.c
 * @author Igor Pontes Tresolavy (tresolavy@usp.br)
 * @brief EP4 da disciplina de MAC0122: índice remissivo de palavras em texto 
 * @version 1.0
 * @date 2021-11-22
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define HASH_TABLE_MAX_SIZE 100000
#define SEED 0xbc9f1d34

/* célula da lista ligada de ocorrências de uma palavra */
typedef struct ocurrenceCell
{
    unsigned long line;/* linha */
    unsigned long numOfApp;/* número de aparições */
    struct ocurrenceCell *next;/* próxima célula */
} ocurrences;

/* célula da lista ligada de uma palavra */
typedef struct wordCell
{ 
  char *word;/* palavra */
  ocurrences *ocurr;/* lista ligada de ocorrências */
  struct wordCell* next;/* pŕoxima palavra */
} words;

typedef enum {FALSE, TRUE} boolean;

/* funções e subrotinas da parte 1 */
void updateHashTable(char*, unsigned long, words***, unsigned long*, unsigned long);
unsigned int scramble(unsigned int);
unsigned int murmurHash(const char*, unsigned long, unsigned int);
void resizeHashTable(words***, unsigned long*, unsigned long);
words* searchForWord(words*, char*);
ocurrences* searchForLine(ocurrences*, unsigned long);
void addOcurrence(words*, unsigned long);
void addToHashTable(words**, unsigned long, char*, unsigned long, unsigned long);
/* ****************** */

/* funções e subrotinas da parte 2 */
void removeHashTableNulls(words***, unsigned long*);
unsigned long amntOfHashTableElements(words**, unsigned long);
void solveCollisions(words***, unsigned long*, unsigned long);
    /* heapsort lexicográfico */
void hashTableHeapsort(words **, unsigned long);
void downgrade(words **, unsigned long, unsigned long);
int caseInsensitiveStrcmp(const char *, const char *);
void heapify (words**, unsigned long);
    /* ******** */
void printHashTable(words**, unsigned long);
void printOcurrencesRecursively(ocurrences*);
void destroyHashTable(words**, unsigned long*);
void destroyOcurrencesRecursively(ocurrences*);
/* ****************** */

int main()
{
    FILE *file;/* arquivo que será analisado */
    words **hashTable;/* tabela hash de palavras */

    unsigned long hashTableSize,/* tamanho da tabela hash */ 
        line, /* linha sendo analisada */
        maxWordSize, /* tamanho do vetor de caracteres de
                        cada palavra do texto, que tem seu 
                        valor atualizado caso uma palavra 
                        com mais que 'maxWordSize' caracteres 
                        seja encontrada. */
        wordSize; /* tamanho da última palavra encontrada no
                     texto, que será adicionada à tabela hash. */

    char c, /* variável que guarda um caractere do texto por vez*/
        *fileName, /* nome do arquivo a ser analisado */
        *wordParser; /* vetor que guarda cada palavra do texto,
                        para posterior adição à tabela hash */
    
    printf("Digite o caminho até o arquivo: ");
    for(wordSize = 0, fileName = malloc(sizeof(char)); (c = getchar()) != EOF && c != '\n'; ++wordSize)
    {
        /* realoca tamanho do vetor para cada caractere de entrada */
        fileName = realloc(fileName, wordSize + 1);
        *(fileName + wordSize) = c; /* adiciona caractere ao vetor */
    }
    *(fileName + wordSize) = '\0'; /* caractere nulo para sinalizar fim de string. */


    if((file = fopen(fileName, "r")) == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        return EXIT_FAILURE;
    }
    else
    {
        /* Libera espaço utilizado pela string "nome do arquivo", pois
        não será mais necessária */
        free(fileName);

        /* Inicializa variáveis */
        maxWordSize = wordSize;/* tamanho do nome do arquivo é 
                                  utilizado como tamanho incial
                                  da string que será usada para
                                  analisar cada palavra do texto */
        hashTableSize = 1;

        /* Alocamento inicial de memória e inicialização da tabela hash*/
        hashTable = malloc(sizeof(words*));
        *hashTable = malloc(sizeof(words));
        *hashTable = NULL;

        /* Aloca espaço para o vetor de palavras no texto e o inicializa com 0s */
        wordParser = calloc(maxWordSize + 1, sizeof(char));
    }


    /* ------- PARTE 1 ------- */

    for(wordSize = 0UL, line = 1UL; /* Inicializador */
       c != EOF; /* Condição de parada */
       wordSize = (isalpha(c))?(wordSize + 1):(0), line += (c == '\n')?(1):(0)) /* incrementos */
    {
        /* se a palavra que está sendo analisada no momento for maior que o tamanho do 
        vetor de caracteres 'wordParser' */
        if(wordSize >= maxWordSize)
        {
            /* realoca e atualiza tamanho do vetor */
            wordParser = realloc(wordParser, sizeof(char)*((maxWordSize *= 2) + 1));
            if(wordParser == NULL)
            {
                printf("Realocamento de memória para palavra falhou.\n");
                return EXIT_FAILURE;
            }
        }

        if(isalpha((c = fgetc(file))))/* se o caracter for uma letra */
            *(wordParser + wordSize) = c;/* adiciona ele ao vetor 'wordParser' */

        else/* caso o caractere não seja uma letra ASCII */
        {
            *(wordParser + wordSize) = '\0';/* uma palavra foi encontrada, então uma string é criada 
                                               adicionando '\0' ao fim de 'wordParser'
                                            */
            if(wordSize > 0)/* Se houver pelo menos uma letra na palavra */
                updateHashTable(wordParser, wordSize, &hashTable, &hashTableSize, line);
        }
    }
    free(wordParser);/* Libera espaço utilizado pela string 'wordParser', pois
                        não será mais necessária */
    fclose(file);/* fecha arquivo analisado */

    /* ------- FIM DA PARTE 1 ------- */

    /* ------- PARTE 2 ------- */

    removeHashTableNulls(&hashTable, &hashTableSize);
    solveCollisions(&hashTable, &hashTableSize, amntOfHashTableElements(hashTable, hashTableSize) - hashTableSize);
    hashTableHeapsort(hashTable, hashTableSize);
    printHashTable(hashTable, hashTableSize);

    /* ------- FIM DA PARTE 2 ------- */

    /* desaloca espaço alocado para a tabela hash e zera 'hashTableSize' */
    destroyHashTable(hashTable, &hashTableSize);
    return 0;
}

void updateHashTable(char *word, unsigned long wordSize, words*** hashTable, unsigned long *hashTableSize, unsigned long line)
/* 
subrotina que adiciona ocorrência na linha 'line' da palavra 'word', de tamanho 'wordSize', 
na tabela hash 'hashTable', de tamanho 'hashTableSize'.
*/
{
    words *foundWord;/* ponteiro que aponta para a posição da palavra 'word' se ela já existir na tabela hash */
    
    ocurrences *foundOcurrence;/* ponteiro que aponta para ocorrência da palavra 'word' na linha 'line' se ela já existir na table hash */
    
    unsigned int hashFunction = murmurHash(word, wordSize, SEED)%HASH_TABLE_MAX_SIZE;/* função Hash da palavra 'word' */

    if(hashFunction >= *hashTableSize)/* se o resultado da função hash for maior ou igual ao tamanho da tabela hash */
    {
        resizeHashTable(hashTable, hashTableSize, hashFunction + 1);/* aumenta o tamanho da tabela hash */
        addToHashTable(*hashTable, hashFunction, word, wordSize, line);/* adiciona a palavra 'word' à posição 
                                                                          'hashFunction' da tabela hash */
    }
    else/* se o resultado da função hash for MENOR que a tabela hash */

        /* se o vetor na posição não for nulo e se a palavra já existir na lista ligada na posição da tabela hash */
        if(*(*hashTable + hashFunction) != NULL && (foundWord = searchForWord(*(*hashTable + hashFunction), word)) != NULL)
    
            /* Se a palavra 'word' já tiver ocorrido na linha 'line' */
            if((foundOcurrence = searchForLine(foundWord->ocurr, line)) != NULL)
                ++(foundOcurrence->numOfApp);/* adiciona uma ocorrência á linha */
    
            else/* Se a palavra NÃO tiver ocorrido na linha 'line' */
                addOcurrence(foundWord, line);/* adiciona linha à lista ligada de ocorrências da palavra 'word' */
    
        else/* se o vetor na posição for nulo OU se a palavra NÃO existir na lista ligada na posição da tabela hash */
            addToHashTable(*hashTable, hashFunction, word, wordSize, line);/* adiciona ocorrência na linha 'line' da nova palavra 'word', 
                                                                              na posição 'hashFunction' da tabela hash */
}

unsigned int scramble(unsigned int k) 
/* 
- Multiplica grupo de 4 bytes por c1; 
- Rotaciona r1 bits de grupo do resultado para a esquerda; 
- Multiplica resultado por c2; 
*/
{
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

unsigned int murmurHash(const char* key, unsigned long len, unsigned int seed)
{
	unsigned int h = seed;
    unsigned int k;
    unsigned long i;

    /* 
    - Multiplica grupo de 4 bytes por c1; 
    - Rotaciona r1 bits de grupo do resultado para a esquerda; 
    - Multiplica resultado por c2; 
    - Atribui à Seed valor da operação lógica OU EXCLUSIVO de Seed com valor do resultado; 
    - Rotaciona r2 bits de Seed para a esquerda; 
    - Atribui à Seed valor da multiplicação aritmética de Seed por m, somado à n. 
    */
    for (i = len >> 2; i > 0; i--) 
    {
        memcpy(&k, key, sizeof(unsigned int));
        key += sizeof(unsigned int);
        h ^= scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    /* 
    - “Arrasta” (logical shift) bytes restantes 8 bits para a esquerda; 
    - Atribui à resultado valor da operação lógica OU com bytes restantes;  
    */
    k = 0;
    for (i = len & 3; i > 0; i--) 
    {
        k <<= 8;
        k |= key[i - 1];
    }

    /* 
    - Atribui à Seed o valor da operação lógica OU EXCLUSIVO de Seed com o resultado anterior; 
    - Atribui à Seed o valor da operação lógica OU EXCLUSIVO de Seed com o valor de Len; 
    - Atribui à Seed o valor da operação lógica OU EXCLUSIVO de Seed com o valor de Seed “arrastado” (logical shift) 16 bits para a direita; 
    - Multiplica Seed por 0x85ebca6b; 
    - Atribui à Seed o valor da operação lógica OU EXCLUSIVO de Seed com o valor de Seed “arrastado” (logical shift) 13 bits para a direita; 
    - Multiplica Seed por 0xc2b2ae35; 
    - Atribui à Seed o valor da operação lógica OU EXCLUSIVO de Seed com o valor de Seed “arrastado” (logical shift) 16 bits para a direita. 
    - Retorna Seed; 
    */

    h ^= scramble(k);
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}

void resizeHashTable(words*** hashTable,unsigned long *oldSize, unsigned long newSize)
/* 
subrotina que aumenta ou diminui o tamanho da tabela hash 'hashTable' de 'oldSize' para 'newSize'.
*/
{
    unsigned long i;

    /* realoca novo tamanho da tabela hash */
    *hashTable = realloc(*hashTable, (newSize)*sizeof(words*));

    /* se o realocamento falhar, encerra o programa */
    if(*hashTable == NULL)
    {
        printf("realocamento de memória para tabela hash falhou.\n");
        exit(EXIT_FAILURE);
    }

    /* se o tamanho novo for maior que o antigo */
    if(*oldSize < newSize)
        /* para cada nova posição, aloca espaço para uma célula de palavras e às inicializa com um vetor nulo  */
        for(i = *oldSize; i < newSize; ++i)
        {
            *(*hashTable + i) = malloc(sizeof(words));
            *(*hashTable + i) = NULL;
        }

    *oldSize = newSize;/* atualiza tamanho da tabela hash */
}

words* searchForWord(words* beggining, char *word)
/* 
função que procura palavra 'word' em uma lista ligada de palavras e retorna ponteiro
para posição, caso a encontre, ou NULL, caso a palavra ainda não tenha ocorrido no texto. 
*/
{
    words *target = NULL;

    /* strcmp: compara duas strings */
    for(target = beggining; target != NULL && strcmp(word, target->word) != 0; target = target->next)
        ;

    return target;
}

ocurrences* searchForLine(ocurrences *beggining, unsigned long line)
/* 
função que procura ocorrência de uma palavra na linha 'line' em uma lista ligada de ocorrências 
e retorna ponteiro para a ocorrência, caso a encontre, ou NULL, caso a palavra ainda não tenha 
ocorrido na linha. 
*/
{
    ocurrences* target = NULL;

    for(target = beggining; target != NULL && target->line != line; target = target->next)
        ;

    return target;
}

void addOcurrence(words* word, unsigned long line)
/* 
subrotina que adiciona ocorrência de palavra 'word' na linha 'line'
*/
{
    ocurrences* newOcurrence = malloc(sizeof(ocurrences));

    /* inicializa valores da ocorrência */
    newOcurrence->line = line;
    newOcurrence->numOfApp = 1;

    /* adiciona a ocorrência ao INÍCIO da lista ligada de ocorrências */
    newOcurrence->next = word->ocurr;
    word->ocurr = newOcurrence;
}

void addToHashTable(words **hashTable, unsigned long position, char *word, unsigned long wordSize, unsigned long line)
/* 
subrotina que adiciona primeira ocorrência da palavra 'word', de 'wordSize' caracteres de comprimento, na linha 'line', 
na posição 'position' da tabela hash 'hashTable' 
*/
{
    words* newWord = malloc(sizeof(words));
    unsigned long i;

    newWord->ocurr = NULL;
    addOcurrence(newWord, line);/* adiciona primeira ocorrência de palavra */
    
    /* copia palavra 'word' para a nova célula da lista ligada de palavras*/
    newWord->word = malloc((wordSize + 1)*sizeof(char));
    for(i = 0UL; i < wordSize; ++i)
        *(newWord->word + i) = *(word + i);
    *(newWord->word + wordSize) = '\0';

    /* adiciona nova palavra ao INÍCIO da lista ligada de palavras */
    newWord->next = *(hashTable + position);
    *(hashTable + position) = newWord;
}

void removeHashTableNulls(words*** hashTable, unsigned long *hashTableSize)
/* 
subrotina que remove posições da tabela 'hashTable', de tamanho 'hashTableSize', que contém vetores nulos,
e diminui tamanho da tabela para que contenha somente vetores não nulos que apontam para listas
ligadas de palavras.
*/
{
    unsigned long nonNullPointerPosition = 0UL, 
            nullPointerPosition = 0UL; 

    /* enquanto a tabela hash não tiver sido completamente analisada */
    while(nonNullPointerPosition < *hashTableSize && nullPointerPosition < *hashTableSize)
    {
        /* encontra a primeira posição com vetor nulo, partindo da última posição encontrada ou do início da tabela */
        while(nullPointerPosition < *hashTableSize && *(*hashTable + nullPointerPosition) != NULL)
            ++nullPointerPosition;

        nonNullPointerPosition = nullPointerPosition + 1;

        /* encontra a primeira posição com vetor não nulo, a partir da última posição com vetor nulo */
        while(nonNullPointerPosition < *hashTableSize && *(*hashTable + nonNullPointerPosition) == NULL)
            ++nonNullPointerPosition;
    
        /* move o vetor não nulo para a posição com vetor nulo */
        if(nonNullPointerPosition < *hashTableSize && nullPointerPosition < *hashTableSize)
        {    
            *(*hashTable + nullPointerPosition) = *(*hashTable + nonNullPointerPosition);
            *(*hashTable + nonNullPointerPosition) = NULL;
            ++nullPointerPosition;
        }
    }

    /* 
    ao final das iterações, as últimas ('hashTableSize' - 'nullPointerPosition') posições 
    da tabela hash 'hashTable' terão somente elementos nulos. Portanto, pode-se eliminar as
    últimas posições.
    */

    /* diminui tamanho da tabela hash 'hashTable' */
    resizeHashTable(hashTable, hashTableSize, nullPointerPosition);
}

unsigned long amntOfHashTableElements(words** hashTable, unsigned long hashTableSize)
/* 
função que conta a quantidade de elementos na tabela hash 'hashTable', 
de tamanho 'hashTableSize', incluindo elementos "colididos".
*/
{
    unsigned long i, amountOfElements;
    words* aux;

    /* 
    enquanto todas as listas ligadas contidas na tabela hash 
    'hashTable' não tiverem sido analisadas. 
    */
    for(i = amountOfElements = 0UL; i < hashTableSize; i++)
    /* 
    conta todos os elementos da lista ligada presentes 
    na posição 'i' da table hash 'hashTable', para todas 
    as posições da tabela 
    */
    {
        aux = ((*(hashTable + i)));
        do
        {
            ++amountOfElements;
            aux = aux->next;
        }while(aux != NULL);
    }

    return amountOfElements;
}

void solveCollisions(words*** hashTable, unsigned long* oldSize, unsigned long collisions)
/* 
subrotina que resolve todas as colisões presentes na tabela hash 'hashTable' 
aumentando o tamanho da tabela e movendo-as para o seu fim.
*/
{
    unsigned long i, j;
    words* aux;


    /* caso não haja colisões, nada é feito */
    if(collisions == 0)
        return;

    /* aumenta tamanho da tabela para suportar as colisões */
    resizeHashTable(hashTable, oldSize, *oldSize + collisions);

    /* move as colisões para o fim da tabela */
    for(i = 0UL, j = *oldSize - collisions; i < *oldSize - collisions; ++i)
        if(((*(*hashTable + i)))->next != NULL)
        {
            aux = ((*(*hashTable + i)));
            do
            {
                aux = aux->next;
                *(*hashTable + j++) = aux;
            
            } while (j < *oldSize && aux->next != NULL);
        }
    
    /* 
    remove ponteiros que apontam para as colisões, transformando 
    a tabela hash 'hashTable' em um vetor linear de ponteiros para
    listas ligadas de somente uma palavra. 
    */
    for(i = 0; i < *oldSize; ++i)
        (*(*hashTable + i))->next = NULL;
}

void hashTableHeapsort(words ** hashTable, unsigned long hashTableSize)
/* 
subrotina que ordena lexicograficamente, isso é, alfabeticamente, 
a tabela hash 'hashTable' utilizando um algoritmo de heapsort.
*/
{
    unsigned long i;
    words* aux;
  
    heapify(hashTable, hashTableSize);
    for(i = hashTableSize - 1; i > 0; --i)
    {
        aux = *hashTable;
        *hashTable = *(hashTable + i);
        *(hashTable + i) = aux;
        downgrade(hashTable, i, 0);
    }
}

void downgrade(words ** hashTable, unsigned long hashTableSize, unsigned long position)
/* 
subrotina que rebaixa o elemento na posição 'position' da tabela hash 'hashTable' 
para uma posição na qual ambos seus filhos sejam lexicograficamente inferiores a si.
*/
{
    words* aux;
    unsigned long parent = position, child = 2*position + 1;
    boolean ok = FALSE;

    while(child < hashTableSize && !ok)
    {
        if(child + 1 < hashTableSize && caseInsensitiveStrcmp((*(hashTable + child + 1))->word, (*(hashTable + child))->word) > 0)
            ++child;
        if(caseInsensitiveStrcmp((*(hashTable + parent))->word, (*(hashTable + child))->word) > 0)
            ok = TRUE;
        else
        {
            aux = *(hashTable + parent);
            *(hashTable + parent) = *(hashTable + child);
            *(hashTable + child) = aux;
            parent = child;
            child = 2*child + 1;
        }
    }
}

int caseInsensitiveStrcmp(const char *p1, const char *p2)
/* 
diferente da função strcmp da biblioteca string.h, esta função
não leva em consideração letras maiúsculas e minúsculas das palavras
*/
{
  unsigned char *s1 = (unsigned char *) p1;
  unsigned char *s2 = (unsigned char *) p2;
  unsigned char c1, c2;
 
  do
  {
      c1 = (unsigned char) toupper((int)*s1++);
      c2 = (unsigned char) toupper((int)*s2++);
      if (c1 == '\0' || c2 == '\0')
      {
            return c1 - c2;
      }
  }
  while (c1 == c2);
 
  return c1 - c2;
}

void heapify (words** hashTable, unsigned long hashTableSize)
/* 
subrotina que efetivamente transforma a tabela hash 'hashTable' em um heap  
*/
{
    unsigned long i;

    for(i = (hashTableSize-2)/2; i > 0; --i)
        downgrade(hashTable, hashTableSize, i);
    downgrade(hashTable, hashTableSize, 0);
}

void printHashTable(words** hashTable, unsigned long hashTableSize)
/* subrotina que imprime a tabela hash 'hashTable'; imprime cada palavra 
incluindo suas listas de ocorrências, em ordem alfabética e crescente, respectivamente 
*/
{
    for(;hashTableSize > 0; ++hashTable, --hashTableSize)
    {
        printf("%s: ", (*hashTable)->word);
        printOcurrencesRecursively((*hashTable)->ocurr);
        printf("\n");
    }
}

void printOcurrencesRecursively(ocurrences* wordOcurrences)
/* 
subrotina que imprime recursivamente uma lista ligada de ocorrências, pois 
o programa insere ocorrências no início de cada lista, de forma que
as listas ligadas de ocorrências sempre estarão em ordem descrescente.
*/
{
    if(wordOcurrences->next != NULL)
        printOcurrencesRecursively(wordOcurrences->next);
    
    if(wordOcurrences->numOfApp > 1)
        printf("%-lu(%-lu) ", wordOcurrences->line, wordOcurrences->numOfApp);
    else
        printf("%-lu ", wordOcurrences->line);
}

void destroyHashTable(words** hashTable, unsigned long* hashTableSize)
/* 
subrotina que destrói todos os elementos da tabela hash 'hashTable', 
liberando a memória previamente alocada para os mesmos.
*/
{
    for(;*hashTableSize > 0; ++hashTable, --(*hashTableSize))
    {
        free((*hashTable)->word);
        destroyOcurrencesRecursively((*hashTable)->ocurr);
        free(*hashTable);
    }
}

void destroyOcurrencesRecursively(ocurrences* wordOcurrences)
/* 
subrotina que destrói todos os elementos de uma lista ligada de ocorrências
recursivamente. 
*/
{
    if(wordOcurrences->next != NULL)
        destroyOcurrencesRecursively(wordOcurrences->next);

    free(wordOcurrences);
}