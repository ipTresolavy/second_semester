#include <stdio.h>
#include <math.h>
#include <limits.h>

/*
A base matemática usada neste EP se encontra neste fórum: 
https://math.stackexchange.com/questions/2006159/leading-digits-of-large-power-of-2 
*/

#define TRUE 1
#define MAX_DIGITS_OF_N 10 /* Quantidade de dígitos em 2 000 000 000 */
#define MAX_POWER 1UL << 63 /* máxima potência alcançável (2 ^ 63) */
#define LOG_OF_2 0.30102999566398119521L

/*
log de 2 na base 10. O sufixo "L" indica o tipo "long double".
Há 21 dígitos após a vírgula pois esta é a máxima precisão 
de conversão de "long double", para texto, para "long double"
novamente permitida pelo ambiente no qual este código foi 
desenvolvido.

É possível verificar a precisão máxima no seu ambiente uti-
lizando a biblioteca <limits.h> e consultando o valor da cons-
tante LDBL_DEC_DIG
*/

int findPower(int n[], int nDigit);

int main()
{
    int n[MAX_DIGITS_OF_N + 1]; /* Um espaço a mais para guardar sinalizador de término */
    int nDigit;

    do
    {
        printf("\nInsira o número da falcatrua: ");

        nDigit = 0;

        while((n[nDigit] = getchar() - '0') >= 0 && 9 >= n[nDigit++])
            ;
        
        if(n[nDigit] == '\n' - '0')
            n[nDigit] = -1; /* sinalizador de término */
        else
        {
            printf("\nHouve um problema com o valor inserido\nFavor verificá-lo\n\n");
            continue;
        }
    }
    while(n[0] != 0 && findPower(n, nDigit));

    printf("\nSaindo...\n\n");
    
    return 0;
}

int findPower(int n[], int nDigit)
{
    long powerDigits, i;
    unsigned long power;
    long double testVar;

    /*Inicialização condizente com a tarefa de encontrar números os quais 
    MAIS da metade dos dígitos foram apagados (mínimo de 3 digitos na potência)*/
    for(powerDigits = 3, power = 7; powerDigits < 2*nDigit; ++power)
        powerDigits = 1 + power*LOG_OF_2;
    
    testVar = floor(pow(2, power - 1));
    while((testVar *=  2) < __LDBL_MAX__)
    {
        i = nDigit - 1;

        testVar /= pow(10, powerDigits - nDigit);
        
        printf("%Lf\n", testVar);
        
        while(n[i] == (int)testVar%10)
            {
                testVar /= 10;
                --i;
                if(testVar == 0)
                {
                    printf("Potência de 2 que começa com os dígitos inseridos: %lu\n", power);
                    return TRUE;
                }
            }
        powerDigits = 1 + (++power)*LOG_OF_2;    
    }

    printf("\nNão há nenhuma potência de 2 que comece com esses dígitos\n\n");

    return TRUE;
}