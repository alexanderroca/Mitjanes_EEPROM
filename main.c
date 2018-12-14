#include <stdio.h>

#define MAX_DIM 12
#define ADDRESS (MAX_DIM - 4)
#define MAX_NUM 100
#define MAX_DEN 10
#define RANG_NUM_MAX 991

unsigned int realitzaMitjana(int numerador, int denominador){
    int mitjana = 0;

    if(denominador > 0)
        mitjana = numerador / denominador;

    if(mitjana >= MAX_NUM){
        mitjana = 0;
    }   //if

    printf("Num: %d - Den: %d - Mitjana: %d\n", numerador, denominador, mitjana);

    return (unsigned int) mitjana;
}

unsigned int calculChecksum(const unsigned int *elements) {
    unsigned int suma = 0;
    int i;
    for(i = 0; i < MAX_DIM; i++){
        suma += elements[i];
    }   //for

    unsigned int ca2 = suma ^ 255;

    ca2 += 1;
    ca2 = ca2 & 0x0FF;
    return ca2;
}

unsigned int binToBCD(unsigned int num){

    unsigned int bcdResult = 0;
    unsigned int shift = 0;
    while (num > 0) {
        bcdResult |= (num % 10) << (shift++ << 2);
        num /= 10;
    }

    return bcdResult;
}

int main() {

     FILE *fp;
     unsigned int address, rang_num = 99;
     unsigned int elements[MAX_DIM];
     int i, j, cont;

    fp = fopen("mitjanes.hex", "w");
    if(fp == NULL) {

        printf("Error, no s'ha pogut crear el fitxer \"mitjanes.txt\"\n");
        return 1;
    }   //if

    else {

        address = 0;
        elements[0] = 8;
        elements[1] = address / 256;
        elements[2] = address - ((address / 256) * 256);
        elements[3] = 0;
        cont = 4;

        for(i = 0; i <= MAX_DEN; i++) {
                for (j = 0; j <= RANG_NUM_MAX; j++) {

                    if (cont == MAX_DIM) {
                        int k;
                        fprintf(fp, ":%02X", elements[0]);

                        for (k = 1; k < MAX_DIM; k++) {
                            fprintf(fp, "%02X", elements[k]);
                        }   //for

                        fprintf(fp, "%02X\n", calculChecksum(elements));
                        address += ADDRESS;

                        printf("Address: %d\n", address);
                        elements[0] = 8;
                        elements[1] = address / 256;
                        elements[2] = address - ((address / 256) * 256);
                        elements[3] = 0;
                        cont = 4;
                    }   //if

                    elements[cont] = binToBCD(realitzaMitjana(j, i));
                    cont++;
                }   //for

                address = (unsigned int) ((1024 * (i + 1)) - 8);
        }   //for


        fprintf(fp, "%s", ":00000001FF");
        fclose(fp);
    }   //else
    return 0;
}