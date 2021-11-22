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

typedef struct ocurrenceCell
{
    unsigned long line;
    unsigned long numOfApp;
    struct ocurrenceCell *next;
} ocurrences;

typedef struct wordCell
{ 
  char *word;
  ocurrences *ocurr;
  struct wordCell* next; 
} words;

typedef enum {FALSE, TRUE} boolean;

void updateHashTable(char*, unsigned long, words***, unsigned long*, unsigned long);
void resizeHashTable(words***, unsigned long*, unsigned long);
words* searchForWord(words*, char*);
ocurrences* searchForLine(ocurrences*, unsigned long);
void addOcurrence(words*, unsigned long);
void addToHashTable(words**, unsigned long, char*, unsigned long, unsigned long);
void removeHashTableNulls(words***, unsigned long*);
unsigned long amntOfHashTableElements(words**, unsigned long);
void solveCollisions(words***, unsigned long*, unsigned long);

void hashTableHeapsort(words **, unsigned long);
void downgrade(words **, unsigned long, unsigned long);
int caseInsensitiveStrcmp(const char *, const char *);
void heapify (words**, unsigned long);

void printHashTable(words**, unsigned long);
void printOcurrencesRecursively(ocurrences*);

void destroyHashTable(words**, unsigned long*);
void destroyOcurrencesRecursively(ocurrences*);

int main()
{
    FILE *file;
    words **hashTable;
    unsigned long hashTableSize, line, maxWordSize, wordSize;
    char c, *fileName;
    char *wordParser;

    printf("Digite o caminho até o arquivo: ");
    for(wordSize = 0, fileName = malloc(sizeof(char)); (c = getchar()) != EOF && c != '\n'; ++wordSize)
    {
        fileName = realloc(fileName, wordSize + 1);
        *(fileName + wordSize) = c;
    }
    *(fileName + wordSize) = '\0';

    if((file = fopen(fileName, "r")) == NULL)
    {
        printf("Erro ao abrir arquivo");
        exit(1);
    }
    else
    {
        /* Libera espaço utilizado pela string "nome do arquivo" */
        free(fileName);

        /* Inicializa variáveis */
        maxWordSize = wordSize;
        hashTableSize = 1;

        /* Alocamento inicial de memória */
        hashTable = malloc(sizeof(words*));
        *hashTable = malloc(sizeof(words));
        *hashTable = NULL;
        wordParser = calloc(maxWordSize + 1, sizeof(char));
    }
    

    for(wordSize = 0UL, line = 1UL; /* Inicializador */
       c != EOF; /* Condição de parada */
       wordSize = (isalpha(c))?(wordSize + 1):(0), line = (c == '\n')?(line + 1):(line)) /* incrementos */
    {
        if(wordSize >= maxWordSize)
        {
            wordParser = realloc(wordParser, sizeof(char)*((maxWordSize *= 2) + 1));
            if(wordParser == NULL)
            {
                printf("realocamento de memória para palavra falhou");
                exit(1);
            }
        }

        if(isalpha((c = fgetc(file))))
            *(wordParser + wordSize) = c;
        else
        {
            *(wordParser + wordSize) = '\0';
            if(wordSize > 0)
                updateHashTable(wordParser, wordSize, &hashTable, &hashTableSize, line);
        }
    }
    free(wordParser);
    fclose(file);

    removeHashTableNulls(&hashTable, &hashTableSize);
    solveCollisions(&hashTable, &hashTableSize, amntOfHashTableElements(hashTable, hashTableSize) - hashTableSize);

    hashTableHeapsort(hashTable, hashTableSize);
    amntOfHashTableElements(hashTable, hashTableSize);

    printHashTable(hashTable, hashTableSize);
    /* Não se esqueca dos free's */
    destroyHashTable(hashTable, &hashTableSize);
    return 0;
}

void updateHashTable(char *word, unsigned long wordSize, words*** hashTable, unsigned long *hashTableSize, unsigned long line)
{
    unsigned long i, hashFunction;
    words *foundWord;
    ocurrences *foundOcurrence;

    for(i = hashFunction = 0UL; i < wordSize; ++i)
        hashFunction += ((*(word + i))%((*(word + i) <= 'Z')?('A' + 10):('a' + 10)))*(wordSize - i);

    if(hashFunction >= *hashTableSize)
    {
        resizeHashTable(hashTable, hashTableSize, hashFunction + 1);
        addToHashTable(*hashTable, hashFunction, word, wordSize, line);
    }
    else
        if(*(*hashTable + hashFunction) != NULL && (foundWord = searchForWord(*(*hashTable + hashFunction), word)) != NULL)
            if((foundOcurrence = searchForLine(foundWord->ocurr, line)) != NULL)
                ++(foundOcurrence->numOfApp);
            else
                addOcurrence(foundWord, line);
        else
            addToHashTable(*hashTable, hashFunction, word, wordSize, line);
}

void resizeHashTable(words*** hashTable,unsigned long *oldSize, unsigned long newSize)
{
    unsigned long i;

    *hashTable = realloc(*hashTable, (newSize)*sizeof(words*));

    if(*oldSize < newSize)
        for(i = *oldSize; i < newSize; ++i)
        {
            *(*hashTable + i) = malloc(sizeof(words));
            *(*hashTable + i) = NULL;
        }

    *oldSize = newSize;
}

words* searchForWord(words* beggining, char *word)
{
    words *target = NULL;


    for(target = beggining; target != NULL && strcmp(word, target->word) != 0; target = target->next)
        ;

    return target;
}

ocurrences* searchForLine(ocurrences *beggining, unsigned long line)
{
    ocurrences* target = NULL;

    for(target = beggining; target != NULL && target->line != line; target = target->next)
        ;

    return target;
}

void addOcurrence(words* word, unsigned long line)
{
    ocurrences* newOcurrence = malloc(sizeof(ocurrences));

    newOcurrence->line = line;
    newOcurrence->numOfApp = 1;
    newOcurrence->next = word->ocurr;
    word->ocurr = newOcurrence;
}

void addToHashTable(words **hashTable, unsigned long position, char *word, unsigned long wordSize, unsigned long line)
{
    words* newWord = malloc(sizeof(words));
    unsigned long i;

    newWord->ocurr = NULL;
    addOcurrence(newWord, line);
    
    newWord->word = malloc((wordSize + 1)*sizeof(char));
    for(i = 0UL; i < wordSize; ++i)
        *(newWord->word + i) = *(word + i);
    *(newWord->word + wordSize) = '\0';

    newWord->next = *(hashTable + position);
    *(hashTable + position) = newWord;
}

void removeHashTableNulls(words*** hashTable, unsigned long *hashTableSize)
{
    unsigned long nonNullPointer = 0UL, 
            nullPointer = 0UL; 

    while(nonNullPointer < *hashTableSize && nullPointer < *hashTableSize)
    {
        while(nullPointer < *hashTableSize && *(*hashTable + nullPointer) != NULL)
            ++nullPointer;

        nonNullPointer = nullPointer + 1;

        while(nonNullPointer < *hashTableSize && *(*hashTable + nonNullPointer) == NULL)
            ++nonNullPointer;
    
        if(nonNullPointer < *hashTableSize && nullPointer < *hashTableSize)
        {    
            *(*hashTable + nullPointer) = *(*hashTable + nonNullPointer);
            *(*hashTable + nonNullPointer) = NULL;
            ++nullPointer;
        }
    }

    resizeHashTable(hashTable, hashTableSize, nullPointer);
}

unsigned long amntOfHashTableElements(words** hashTable, unsigned long hashTableSize)
{
    unsigned long i, amountOfElements;
    words* aux;

    for(i = amountOfElements = 0UL; i < hashTableSize; i++)
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
{
    unsigned long i, j;
    words* aux;

    resizeHashTable(hashTable, oldSize, *oldSize + collisions);

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
    

    for(i = 0; i < *oldSize; ++i)
        (*(*hashTable + i))->next = NULL;
}

void hashTableHeapsort(words ** hashTable, unsigned long hashTableSize)
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
{
    unsigned long i;

    for(i = (hashTableSize-2)/2; i > 0; --i)
        downgrade(hashTable, hashTableSize, i);
    downgrade(hashTable, hashTableSize, 0);
}

void printHashTable(words** hashTable, unsigned long hashTableSize)
{
    for(;hashTableSize > 0; ++hashTable, --hashTableSize)
    {
        printf("%s: ", (*hashTable)->word);
        printOcurrencesRecursively((*hashTable)->ocurr);
        printf("\n");
    }
}

void printOcurrencesRecursively(ocurrences* wordOcurrences)
{
    if(wordOcurrences->next != NULL)
        printOcurrencesRecursively(wordOcurrences->next);
    
    if(wordOcurrences->numOfApp > 1)
        printf("%-lu(%-lu) ", wordOcurrences->line, wordOcurrences->numOfApp);
    else
        printf("%-lu ", wordOcurrences->line);
}

void destroyHashTable(words** hashTable, unsigned long* hashTableSize)
{
    for(;*hashTableSize > 0; ++hashTable, --(*hashTableSize))
    {
        free((*hashTable)->word);
        destroyOcurrencesRecursively((*hashTable)->ocurr);
        free(*hashTable);
    }
}

void destroyOcurrencesRecursively(ocurrences* wordOcurrences)
{
    if(wordOcurrences->next != NULL)
        destroyOcurrencesRecursively(wordOcurrences->next);

    free(wordOcurrences);
}