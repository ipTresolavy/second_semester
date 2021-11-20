#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
boolean compareWords(char*, char*);
void colapseHashTable(words***, unsigned long*);
unsigned long amntOfHashTableElements(words**, unsigned long);
void solveCollisions(words***, unsigned long*, unsigned long);
 

int main()
{
    FILE *file;
    words **hashTable;
    unsigned long hashTableSize, line, maxWordSize, wordSize;
    char c, *fileName;
    char *wordParser;

    /* printf("Digite o caminho até o arquivo: "); */
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
       wordSize = (isalnum(c))?(wordSize + 1):(0), line = (c == '\n')?(line + 1):(line)) /* incrementos */
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

        if(isalnum((c = fgetc(file))))
            *(wordParser + wordSize) = c;
        else
        {
            *(wordParser + wordSize) = '\0';
            if(wordSize > 0)
                updateHashTable(wordParser, wordSize, &hashTable, &hashTableSize, line);
        }
    }
    fclose(file);

    colapseHashTable(&hashTable, &hashTableSize);
    /* printf("%lu\n", amntOfHashTableElements(hashTable, hashTableSize)); */
    solveCollisions(&hashTable, &hashTableSize, amntOfHashTableElements(hashTable, hashTableSize) - hashTableSize);

    amntOfHashTableElements(hashTable, hashTableSize);

    /* Não se esqueca dos free's */
    return 0;
}

void updateHashTable(char *word, unsigned long wordSize, words*** hashTable, unsigned long *hashTableSize, unsigned long line)
{
    unsigned long i, hashFunction;
    words *foundWord;
    ocurrences *foundOcurrence;

    for(i = hashFunction = 0UL; i < wordSize; ++i)
        hashFunction += ((*(word + i))%((*(word + i) <= '9')?('0'):((*(word + i) <= 'Z')?('A' + 10):('a' + 10))))*(wordSize - i);

    if(hashFunction >= *hashTableSize)
    {
        /* Aumenta o tamanho da tabela Hash e inicializa os ponteiros com o valor NULL */
        /* *hashTable = resizeHashTable(*hashTable, *hashTableSize, hashFunction + 1); */
        /* *hashTable = realloc(*hashTable, (hashFunction + 1)*sizeof(words*));
        for(i = *hashTableSize; i <= hashFunction; ++i)
        {
            *(*hashTable + i) = malloc(sizeof(words));
            *(*hashTable + i) = NULL;
        }
        *hashTableSize = hashFunction + 1; */
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

    for(target = beggining; target != NULL && !compareWords(word, target->word); target = target->next)
        ;

    return target;
}

boolean compareWords(char *word1, char *word2)
{
    while(*word1 != '\0' && *word2 != '\0' && *word1 == *word2)
    {
        ++word1;
        ++word2;
    }

    return (*word1 == '\0' && *word2 == '\0')?(TRUE):(FALSE);
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

void colapseHashTable(words*** hashTable, unsigned long *hashTableSize)
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


    /* printf("%s\n", ((*(*hashTable + 1293))->word)); */
    
    resizeHashTable(hashTable, hashTableSize, nullPointer);

    /* *hashTable = realloc(*hashTable, (nullPointer)*sizeof(words*));
    *hashTableSize = nullPointer; */
    /* for(nonNullPointer = 0; nonNullPointer < nullPointer; nonNullPointer++)
    {
        aux = ((*(*hashTable + nonNullPointer)));
        do
        {
            printf("%s|", aux->word);
            aux = aux->next;
        }while(aux != NULL);

    } */
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
            printf("%s\n", aux->word);
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
    {
        aux = ((*(*hashTable + i)));
        if(aux->next != NULL)
            do
            {
                aux = aux->next;
                *(*hashTable + j++) = aux;
            
            } while (j < *oldSize && aux->next != NULL);
    }

    for(i = 0; i < *oldSize; ++i)
        (*(*hashTable + i))->next = NULL;

}