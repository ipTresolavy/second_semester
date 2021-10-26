#include <stdio.h>
#include <math.h>
#include <limits.h>

/*
A base matemática usada neste EP se encontra neste fórum: 
https://math.stackexchange.com/questions/2006159/leading-digits-of-large-power-of-2 
*/

#define TRUE 1
#define FALSE 0
#define MAX_DIGITS_OF_N 10 /* Quantidade de dígitos em 2 000 000 000 */
#define MAX_POWER  /* máxima potência alcançável (2 ^ 63) */
#define LOG10_OF_2 0.30102999566398119521L


int main()
{
    long power = 2000000000;
    unsigned long n = 9876543210;
    
    printf("%f\n", pow(10.0, power*LOG10_OF_2 - 1 - floor(power*LOG10_OF_2) + MAX_DIGITS_OF_N));

    if(n == (unsigned long)pow(10.0, power*LOG10_OF_2 - 1 - floor(power*LOG10_OF_2) + MAX_DIGITS_OF_N))
        printf("True\n");
    else
        printf("False\n");

    return 0;
}