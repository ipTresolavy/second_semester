/* 

Dado um vetor v com n elementos, ordene o vetor em 
ordem crescente, ou seja, 

v[0] <= v[1] <= ... <= v[n-1]


*/ 
#include <stdio.h>
#include <stdlib.h>

#define BMAG "\e[1;35m"
#define reset "\e[0m"

void rebaixa(int *, int, int, int); 
/* void rebaixa(words **, unsigned long, unsigned long); */
void MEUheapsort(int *, int);
/* void MEUheapsort(words **, unsigned long); */
void heapfica (int *, int); 
/* void heapfica (words**, unsigned long);  */
void printVector(int *vector, int m, int colourfulIndex);

int main(){
  int *v;
  int i, n;
  printf("Digite n > 0: ");
  if(!scanf("%d", &n))
    return -1;
  v = malloc (n*sizeof(int)); 
  for (i = 0; i < n; i++) /* v[i] = 1000 * ((double) rand() / RAND_MAX); */
    if(!scanf("%d", &v[i]))
      return -1;

  printf("Vetor incial: ");
  for (i = 0; i < n; i++) printf("%2d ", v[i]);
  printf("\n\n");

  MEUheapsort(v,n);

  printf("Vetor final: ");
  for (i = 0; i < n; i++) printf("%2d ", v[i]);
  printf("\n");
  return 0;
}

void MEUheapsort(int * v, int n){
  int i, aux;
  heapfica (v, n);
  for (i = n-1; i > 0; i--){
    aux = v[0];
    v[0] = v[i];
    v[i] = aux;
    printf("Posição 0 rebaixada para ");
    rebaixa (v, i, 0, n);
  }
  /* 
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
   */
}

/*  2i -> 4i -> 8i -> ... -> 2ˆk i > n
    2^k > n/i, ou seja, k ~ log_2 n - log_2 i  */ 

void rebaixa (int *v, int n, int i, int n2){
  /* rebaixa o elemento na posição i do heap que tem n 
     elementos  */
  int aux, pai = i, filho = 2*i + 1, ok = 0;
  while (filho < n && !ok){
    if (filho + 1 < n && v[filho + 1] > v[filho])
      filho = filho + 1;
    if (v[pai] > v[filho])
      ok = 1;
    else{
      aux = v[pai];
      v[pai] = v[filho];
      v[filho] = aux;
      pai = filho;
      filho = 2*filho + 1;
    }
  }
    /* 
    words* aux;
    unsigned long parent = position, child = 2*position + 1;
    boolean ok = FALSE;

    while(child < hashTableSize && !ok)
    {
        if(child + 1 < hashTableSize && strcmp(*(hashTable + child + 1)->word, *(hashTable + child)->word) > 0)
            ++child;
        if(strcmp(*(hashTable + parent)->word, *(hashTable + child)->word) > 0)
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
     */
}


/* executa em tempo linear O(n) */ 
void heapfica (int *v, int n){
  int i;
  for (i = (n-2)/2; i >= 0; i--)
  {
    printf("Posição %2d rebaixada para ", i);
    rebaixa(v, n, i, n);
  }
  /* 
  unsigned long i;

  for(i = (hashTableSize-2)/2; i >= 0; --i)
      downgrade(hashTable, hashTableSize, i);
   */
}

void printVector(int *vector, int m, int colourfulIndex)
{
    int i;

    for(i = 0; i < m; ++i)
      if(i != colourfulIndex)
        printf("%2d ", vector[i]);
      else
        printf(BMAG "%2d " reset, vector[i]);

    printf("\n\n");
}
