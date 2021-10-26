#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define MAX_DIGITS_OF_N 10 /* Quantidade de dígitos em 2 000 000 000 */
#define LOG10_OF_2 0.30102999566398119521L /* log de 2 na base 10. */

char digitosPot[UINT_MAX];

int processaN(int n);
int achaPotencia();
unsigned long doisElevadoAPot(char* digitosPot, unsigned long tamPot);


int main()
{
    int n;

    printf("\nPotência máxima do programa: 2^%lu\n", ULONG_MAX - 1);

    do
    {
        printf("\nInsira o número da falcatrua: ");
        
    } while (scanf("%d", &n) && processaN(n));


    if(n != 0)
        printf("\nHouve um problema com o valor inserido\nFavor verificá-lo\n\n");
    else
        printf("\nSaindo...\n\n");


    return 0;
}

void imprimeNumerao(char num[], int tamNum){
    
    int temp = tamNum - 5;

    while( tamNum >= 0 && tamNum>=temp){/*imprime cada digito*/
        printf("%c", num[tamNum]);
        tamNum--;
    }
}

int processaN(int n)
{
    int i;
    /*char* digitosPot;*/
    int digitosN[MAX_DIGITS_OF_N];
    unsigned int tamN;
    unsigned long tamPot, pot, j;

    /* Interrompe a execução do programa */
    if(n <= 0)
        return FALSE;
    
    /* Encontra quantidade de dígitos em n */
    for(tamN = 0; tamN < MAX_DIGITS_OF_N && n != 0; ++tamN)
    {
        digitosN[tamN] = n%10;
        n /= 10;
    }

    tamPot = 3UL;
    pot = 7UL;
    /*digitosPot = (char *)malloc( ( tamDigPot = ((tamPot*2 + 1)) )*sizeof(char)  );*/
    digitosPot[0] = '8';
    *(digitosPot + 1) = '2';
    *(digitosPot + 2) = '1';
    *(digitosPot + 3) = '0';

    while(tamPot <= 2*tamN)
    {
        tamPot = doisElevadoAPot(digitosPot, tamPot);
        ++pot;
    }

    while(TRUE)
    {
       for(i = tamN - 1, j = tamPot - 1; i >= 0 && digitosN[i] == digitosPot[j] - '0'; --i, --j)
            ;
        
        if(i == -1)
        {
            printf("Potência de 2 que começa com os dígitos inseridos: %lu\n", pot);
            return TRUE;
        }
        else
        {
            tamPot = doisElevadoAPot(digitosPot, tamPot);
            ++pot;
        }
    }

    /*free(digitosPot);*/
}


unsigned long doisElevadoAPot(char* digitosPot, unsigned long tamPot)
{
    unsigned int i, exc;

    for(i = exc = 0; i <= tamPot; i++)
    {   
        /*em qualquer soma de dois números, é impossível que o resultado tenha dois dígitos a mais que o número com maior quantidade de dígitos*/
        if(digitosPot[i] - '0' + digitosPot[i] - '0' +  exc >= 10)
        {   
            /*checa se soma dos dois digitos e do excedente da soma anterior é um número de dois dígitos*/
            digitosPot[i] = (digitosPot[i] - '0' + digitosPot[i] -'0' +  exc)%10 + '0';/*armazena unidade da soma no vetor*/
            exc = 1;/*dezena do resultado vai para a próxima soma*/
        }
        else
        {
            digitosPot[i] = digitosPot[i] - '0' + digitosPot[i] -'0' +  exc + '0';
            exc = 0;/*Caso a soma seja menor que 10, não há excedente*/
        }
    }

    if(digitosPot[--i] != '0')
    {
        digitosPot[++i] = '0';
        ++(tamPot);
    }

    return tamPot;
}