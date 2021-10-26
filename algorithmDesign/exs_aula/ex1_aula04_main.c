#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>

int casa(char, char);

int main()
{
    char entrada[100], caraDoTopo;
    int i, naoPerdeu = 1;
    pilha * duracell;


    printf("Digite a sequência desejada: ");
    if(scanf("%[^\n]", entrada))
    {
        duracell = criaPilha(10);

        for(i = 0; entrada[i] != '\0' && naoPerdeu; ++i)
        {
            if(entrada[i] == '(' || entrada[i] == '[' || entrada[i] == '{')
                empilha(duracell, entrada[i]);
            else 
            {
                if (entrada[i] == ')' || entrada[i] == ']' || entrada[i] == '}')
                {
                    if(!pilhaVazia(duracell))
                    {
                        caraDoTopo = desempilha(duracell);
                        if(!casa(entrada[i], caraDoTopo))
                        {
                            naoPerdeu = 0;
                            break;
                        }
                    }
                    else
                    {   
                        naoPerdeu = 0;
                        break;
                    }
                }
            }
        }

        if(pilhaVazia(duracell) && naoPerdeu)
            printf("A sequência é bem formada\n");
        else
            printf("A sequência NÃO é bem formada\n");
    }

    return 0;
}

int casa(char c1, char c2)
{
    if((c2 == '(' && c1 == ')') || (c2 == '[' && c1 == ']') || (c2 == '{' && c1 == '}'))
        return 1;
    else
        return 0;
}