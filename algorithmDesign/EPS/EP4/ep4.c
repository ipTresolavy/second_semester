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
words** resizeHashTable(words**, unsigned long, unsigned long);
words* searchForWord(words*, char*);
ocurrences* searchForLine(ocurrences*, unsigned long);
void addOcurrence(words*, unsigned long);
void addToHashTable(words**, unsigned long, char*, unsigned long, unsigned long);
boolean compare(char*, char*);
void colapseHashTable(words***, unsigned long*);

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
       (c = fgetc(file)) != EOF; /* Condição de parada */
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

        if(isalnum(c))
            *(wordParser + wordSize) = c;
        else
        {
            *(wordParser + wordSize) = '\0';
            if(wordSize > 0)
                updateHashTable(wordParser, wordSize, &hashTable, &hashTableSize, line);
        }
    }
    fclose(file);

    printf("%s\n", ((*(hashTable + 506))->word));
    colapseHashTable(&hashTable, &hashTableSize);
    printf("%s\n", ((*(hashTable + 2))->word));

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
        *hashTable = realloc(*hashTable, (hashFunction + 1)*sizeof(words*));
        for(i = *hashTableSize; i <= hashFunction; ++i)
        {
            *(*hashTable + i) = malloc(sizeof(words));
            *(*hashTable + i) = NULL;
        }
        *hashTableSize = hashFunction + 1;
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

words** resizeHashTable(words** hashTable,unsigned long oldSize, unsigned long newSize)
{
    words** newHashTable = calloc(newSize, sizeof(words*));
    unsigned long smallestSize = (oldSize < newSize)?(oldSize):(newSize);

    while(0 < --smallestSize)
        *(newHashTable + oldSize) = *(hashTable + oldSize);
    *newHashTable = *hashTable;

    free(hashTable);

    return newHashTable;
}

words* searchForWord(words* beggining, char *word)
{
    words *target = NULL;

    for(target = beggining; target != NULL && !compare(word, target->word); target = target->next)
        ;

    return target;
}

boolean compare(char *word1, char *word2)
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


    printf("%s\n", ((*(*hashTable + 1))->ocurr == NULL)?("yes"):("bruh"));
    printf("%s\n", ((*(*hashTable + 1))->word));

    /* *hashTable = resizeHashTable(*hashTable, *hashTableSize, nullPointer);
    *hashTableSize = nullPointer; */
}