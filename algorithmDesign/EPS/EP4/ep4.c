#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct ocurrenceCell
{
    unsigned long line;
    unsigned long numOfApp;
    struct ocurrenceCell *next;
} ocurrences;

typedef char item;
typedef struct wordCell
{ 
  item *word;
  ocurrences *ocurr;
  struct wordCell* next; 
} words;

typedef enum {FALSE, TRUE} boolean;

void updateHashTable(item*, unsigned long, words**, unsigned long*, unsigned long);
words** resizeHashTable(words**, unsigned long, unsigned long, unsigned long);
words* searchForWord(words**, unsigned long, item*);
ocurrences* searchForLine(words*, unsigned long);
void addOcurrence(words*, unsigned long);
void addToHashTable(words*, item*, unsigned long, unsigned long);
boolean compare(item*, item*);

int main()
{
    FILE *file;
    words **hashTable;
    unsigned long hashTableSize, line, maxWordSize, wordSize;
    char c, *fileName;
    item *wordParser;

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
        /* Libera espaço utilizado pela string nome do arquivo */
        free(fileName);

        /* Inicializa variáveis */
        maxWordSize = wordSize;
        hashTableSize = 1;

        /* Alocamento inicial de memória */
        hashTable = calloc(hashTableSize, sizeof(words*));
        *hashTable = calloc(1, sizeof(words));
        (*hashTable)->word = calloc(maxWordSize + 1, sizeof(item));
        (*hashTable)->next = NULL;
        (*hashTable)->ocurr = NULL;
        wordParser = calloc(maxWordSize + 1, sizeof(item));
        /* (*hashTable)->ocurr = calloc(1, sizeof(ocurrences));
        (*hashTable)->ocurr->next = calloc(1, sizeof(struct ocurrenceCell));
        (*hashTable)->next = calloc(1, sizeof(struct wordCell)); */
    }
    

    for(wordSize = 0UL, line = 1UL; /* Inicializador */
       (c = fgetc(file)) != EOF; /* Condição de parada */
       wordSize = (isalnum(c))?(wordSize + 1):(0), line = (c == '\n')?(line + 1):(line)) /* incrementos */
    {
        if(wordSize >= maxWordSize)
        {
            wordParser = realloc(wordParser, sizeof(item)*(maxWordSize *= 2));
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
            updateHashTable(wordParser, wordSize, hashTable, &hashTableSize, line);
        }   
    
    }

    fclose(file);
    /* Não se esqueca dos free's */
    return 0;
}

void updateHashTable(item* word, unsigned long wordSize, words** hashTable, unsigned long *hashTableSize, unsigned long line)
{
    unsigned long i, hashFunction;
    words *foundWord;
    ocurrences *foundOcurrence;

    for(i = hashFunction = 0UL; i < wordSize; ++i)
        hashFunction += (*(word + i) - 'A')*(wordSize - i);

    if(hashFunction > *hashTableSize)
    {
        hashTable = resizeHashTable(hashTable, wordSize, *hashTableSize, hashFunction + 1);
        *hashTableSize = hashFunction + 1;
    }

    if(*(hashTable + hashFunction) != NULL && (foundWord = searchForWord(hashTable, *hashTableSize, word)) != NULL)
        if((foundOcurrence = searchForLine(foundWord, line)) != NULL)
            ++(foundOcurrence->numOfApp);
        else
            addOcurrence(foundWord, line);
    else
        addToHashTable(*(hashTable + hashFunction), word, wordSize, line);
}

words** resizeHashTable(words** hashTable, unsigned long wordSize, unsigned long oldSize, unsigned long newSize)
{
    words** newHashTable = calloc(newSize, sizeof(words*));
    unsigned long i;


    for(i = 0UL; i < newSize; ++i)
        *(newHashTable + i) = calloc(1, sizeof(words));

    for(i = 0UL; i < newSize; ++i)
    {
        (*(newHashTable + i))->word = calloc(wordSize + 1, sizeof(item));
        (*(newHashTable + i))->next = NULL;
        (*(newHashTable + i))->ocurr = NULL;
    }

    for(i = 0UL; i < oldSize; ++i)
        *(newHashTable + i) = *(hashTable + i);
    /* free(hashTable); */

    return newHashTable;
}

words* searchForWord(words** hashTable, unsigned long hashTableSize, item* word)
{
    unsigned long i;
    words* iterator = NULL;


    if((*(hashTable + 1))->word)
        for(i = 0UL; i < hashTableSize; ++i)
        if(*(hashTable + i) != NULL)
            for(iterator = *(hashTable + i); 
            iterator != NULL 
            && 
            iterator->word != NULL 
            && 
            compare(iterator->word, word) != 0; 
            iterator = iterator->next)
                ;
    
    return iterator;
}

boolean compare(item* word1, item* word2)
{
    while(*word2 != '\0' && *word1 != '\0' && *word1 == *word2)
    {
        ++word1;
        ++word2;
    }

    return (*word2 == '\0' && *word1 == '\0')?(TRUE):(FALSE);
}

ocurrences* searchForLine(words* word, unsigned long line)
{
    ocurrences* iterator = NULL;

    for(iterator = word->ocurr; iterator != NULL && iterator->line != line; iterator = iterator->next)
        ;
    
    return iterator;
}

void addOcurrence(words* word, unsigned long line)
{
    ocurrences *newOcurr = malloc(sizeof(ocurrences));

    newOcurr->line = line;
    newOcurr->numOfApp = 1;
    newOcurr->next = word->ocurr;
    word->ocurr = newOcurr;
}

void addToHashTable(words* hashTablePosition, item* newWord, unsigned long newWordSize, unsigned long line)
{
    words* newString = malloc(sizeof(words));
    unsigned long i;

    newString->word = malloc((newWordSize+1)*sizeof(char));
    for(i = 0UL; *(newWord + i) != '\0'; ++i)
        *(newString->word + i) = *(newWord + i);
    *(newString->word + i) = '\0';

    addOcurrence(hashTablePosition, line);
    newString->next = hashTablePosition;
    hashTablePosition = newString;
}