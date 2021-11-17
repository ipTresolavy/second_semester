/* 

Listas Ligadas 

*/ 

#include <stdio.h>
#include <stdlib.h>

typedef int item; 
typedef struct cel {
  item info;
  struct cel * prox;
} celula;

/* lê uma sequencia de números diferentes de zero e 
   coloca numa lista */ 

void imprimelista (celula *);
celula * busca (celula *, item);
celula * buscaRec (celula *, item);
celula * insereNoInicio (celula *, item);
void insereNoInicioVoid (celula **, item);
celula * insereNoFim (celula *, item); 
celula * insereNoFimRec (celula *, item); 

int main(){
  celula * inicio = NULL;
  int numero;
  printf("Digite um número: "); 
  scanf("%d", &numero);
  while (numero != 0){
    inicio = insereNoFim (inicio, numero); 
    printf("Digite o próx número: ");
    scanf("%d", &numero); 
  }
  imprimelista (inicio);
  printf("Digite um número: ");
  scanf("%d", &numero);
  if (busca (inicio, numero))
    printf("%d ocorre na lista\n", numero);
  else
    printf("%d não está na lista\n", numero); 
  return 0;
}

void imprimelista (celula * inicio){
  celula * aponta = inicio;
  while (aponta != NULL){
    printf("[%3d] ", aponta->info);
    aponta = aponta->prox;
  }
  printf("\n");
} 

celula * busca(celula * inicio, item x){
  celula * p;
  for (p = inicio; p != NULL && p->info != x; p = p->prox);
  return p;
}

celula * buscaRec(celula * inicio, item x){
  if (inicio == NULL || inicio->info == x)
    return inicio;
  return buscaRec (inicio->prox, x);
} 

celula * insereNoInicio (celula * inicio, item x){
  celula * novo = malloc (sizeof(celula));
  novo->info = x;
  novo->prox = inicio;
  return novo;
} 

void insereNoInicioVoid (celula ** inicio, item x){
  celula * novo = malloc (sizeof(celula));
  novo->info = x;
  novo->prox = *inicio;
  *inicio = novo;
} 

celula * insereNoFimRec (celula * inicio, item x){
  celula * novo; 
  if (inicio == NULL){
    novo = malloc(sizeof(celula));
    novo->info = x;
    novo->prox = NULL;
    return novo;
  }
  inicio->prox = insereNoFim (inicio->prox, x);
  return inicio;
} 

celula * insereNoFim (celula * inicio, item x){
  celula * novo = malloc (sizeof(celula));
  celula * p, *ant; 
  novo->info = x;
  novo->prox = NULL;
  for (ant = NULL, p = inicio; p != NULL; ant = p, p = p->prox);
  if (ant != NULL)
    ant->prox = novo;
  else
    inicio = novo;
  return inicio;
}

celula * insereNoFim (celula * inicio, item x){
  celula * novo = malloc (sizeof(celula));
  celula * p;
  novo->info = x;
  novo->prox = NULL;
  for (p = inicio; p != NULL && p->prox != NULL; p = p->prox);
  if (p != NULL)
    p->prox = novo;
  else
    inicio = novo; 
  return inicio;
}
  
