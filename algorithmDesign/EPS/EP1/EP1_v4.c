#include <stdio.h>
#include <math.h>

/*
A base matemática usada neste EP se encontra neste fórum: 
https://math.stackexchange.com/questions/2006159/leading-digits-of-large-power-of-2 
*/

#define TRUE 1
#define FALSE 0       
#define MAX_PRECISION 1907710532UL
#define LOG10_OF_2 0.30102999566398119521L /* log de 2 na base 10. */

int findPower(int n);

int main()
{
    int n;

    printf("\nPotência máxima aproximada do programa: 2 ^ %lu\n", MAX_PRECISION);

    do
    {
        printf("\nInsira o número da falcatrua: ");
        
    } while (scanf("%d", &n) && findPower(n));


    if(n != 0)
        printf("\nHouve um problema com o valor inserido\nFavor verificá-lo\n\n");
    else
        printf("\nSaindo...\n\n");


    return 0;
}

int findPower(int n)
{
    unsigned int i, nDigits;
    unsigned long power, powerDigits;
    
    /* Interrompe a execução do programa */
    if(n <= 0)
        return FALSE;

    /* Encontra quantidade de dígitos em n, sem alterar seu valor */
    for(nDigits = 0, i = n; i != 0; ++nDigits)
        i /= 10;
    
    /* Inicialização condizente com a tarefa de encontrar números os quais 
    MAIS da metade dos dígitos foram apagados (mínimo de 3 digitos na potência) */
    for(powerDigits = 3, power = 7; powerDigits < 2*nDigits; ++power)
        powerDigits = 1 + (power*LOG10_OF_2);
    
    while(power <= MAX_PRECISION)
    {
        /* "int" força o truncamento do número */
        if(n == (int)pow(10.0, power*LOG10_OF_2 - powerDigits + nDigits))
        {
            printf("Potência de 2 que começa com os dígitos inseridos: 2 ^ %lu\n", power);
            return TRUE;
        }
        else
            powerDigits = 1 + ((++power)*LOG10_OF_2);
    }
    
    printf("Não há nenhuma potência de 2, no intervalo [128, 2 ^ %lu], que comece com esses dígitos\n", MAX_PRECISION);
    return TRUE;
}