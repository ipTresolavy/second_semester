#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

#define MAX 100
#define TRUE 1
#define FALSE 0

int ehOperando(char c);
int prec(char c);

int main()
{
    char c, seq[MAX];
    int i = 0;
    pilha *patio;
    patio = criaPilha();

    if(scanf("%[^\n]", seq))
    {
        while(seq[i] != '\0')
        {
            if(ehOperando(seq[i]))
                printf("%c", seq[i]);
            else if(seq[i] == '(')
                empilha(patio, seq[i]);
            else if(seq[i] == ')')
            {
                c = desempilha(patio);
                while (c != '(')
                {
                    printf("%c", c);
                    c = desempilha(patio);
                }
            }
            else if(pilhaVazia(patio))
                    empilha(patio, seq[i]);
                else
                {
                    c = topoDaPilha(patio);
                    while(!pilhaVazia(patio) && prec(c) >= prec(seq[i]))
                    {
                        c = desempilha(patio);
                        printf("%c", c);
                        if(!pilhaVazia(patio))
                            c = topoDaPilha(patio);
                    }
                    empilha(patio, seq[i]);
                }
            i++;
        }
        while(!pilhaVazia(patio))
            printf("%c", (c = desempilha(patio)));
    }

    printf("\n");
    return 0;
}

int ehOperando(char c)
{
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return TRUE;
    else
        return FALSE;
}

int prec(char c)
{
    enum precedence{parenthesis = 0, soma = 1, subtracao = 1, multipl = 2, divis = 2};

    switch (c)
    {
    case '(':
        return parenthesis;
        break;
    
    case '+':
        return soma;
        break;

    case '-':
        return subtracao;
        break;

    case '*':
        return multipl;
        break;
    
    case '/':
        return divis;
        break;
    
    default:
        return FALSE;
        break;
    }

}