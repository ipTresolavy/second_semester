#include <stdio.h>
#include "fila.h"

void ratinho(int **lab, int m, int n, pos rato, pos queijo);


int main()
{
    return 0;
}

void ratinho(int **lab, int m, int n, pos rato, pos queijo)
{
    fila* caminho = criaFila(m + n);
    item atual, prox;

    atual = rato;
    insereFila(caminho, atual);
    lab[atual.l][atual.c] = 1;

    while(atual.l != queijo.l || atual.c != queijo.c)
    {
        if(filaVazia(caminho))
        {
            destroiFila(caminho);
            return;
        }
        atual = primeiroFila(caminho);
        prox.l = atual.l + 1;
        prox.c = atual.c;
        if(prox.l < m && lab[prox.l][prox.c] == 0)
        {
            lab[prox.l][prox.c] = lab[atual.l][atual.c] + 1;
            insereFila(caminho, prox);
        }

        prox.l = atual.l - 1;
        prox.c = atual.c;
        if(prox.l >= 0 && lab[prox.l][prox.c] == 0)
        {
            lab[prox.l][prox.c] = lab[atual.l][atual.c] + 1;
            insereFila(caminho, prox);
        }

        prox.l = atual.l;
        prox.c = atual.c + 1;
        if(prox.c < n && lab[prox.l][prox.c] == 0)
        {
            lab[prox.l][prox.c] = lab[atual.l][atual.c] + 1;
            insereFila(caminho, prox);
        }

        prox.l = atual.l;
        prox.c = atual.c - 1;
        if(prox.l >= 0 && lab[prox.l][prox.c] == 0)
        {
            lab[prox.l][prox.c] = lab[atual.l][atual.c] + 1;
            insereFila(caminho, prox);
        }
    }

}