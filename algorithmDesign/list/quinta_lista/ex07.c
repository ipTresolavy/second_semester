#ifndef IMPRIME_PROPORCIONAL_A_PROFUNDIDADE
#define IMPRIME_PROPORCIONAL_A_PROFUNDIDADE
#include <stdio.h>

typedef struct cel {
  int info;
  struct cel * esq;
  struct cel * dir;
} no;

/* valor inicial de "profundidade" deve ser 0 */
void imprimeProporcional(no *raiz, int profundidade)
{
    int i;

    if(raiz != NULL)
    {
        imprimeProporcional(raiz->dir, ++profundidade);
        printf("\n");

        for(i = 0; i < profundidade; ++i)
            printf("\t");

        printf("%d\n", raiz->info);
    
        imprimeProporcional(raiz->esq, profundidade);
    }
}

#endif
