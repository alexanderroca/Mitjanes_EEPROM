#include <stdio.h>

#define MAX_DIM 12
#define ADDRESS (MAX_DIM - 4)
#define MAX_NUM 100
#define MAX_DEN 10

unsigned int realitzaMitjana(int numerador, int denominador){
    int mitjana = 0;

    if(denominador > 0)
        mitjana = numerador / denominador;

    return (unsigned int) mitjana;
}

void inicialitzacioElements(unsigned int *elements){
    int i;

    for(i = 0; i < MAX_DIM; i++){
        elements[i] = 0;
    }   //for
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
    unsigned int res;
    unsigned int aux;
    aux = num;
    aux = aux / 10;
    res = aux << 4;

    res = res | (num - aux);

    return res;
}

int main() {
     FILE *fp;
     unsigned int address;
     unsigned int elements[MAX_DIM];
     int i, j, cont;

    fp = fopen("mitjanes.hex", "w");
    if(fp == NULL) {

        printf("Error, no s'ha pogut crear el fitxer \"mitjanes.txt\"\n");
        return 1;
    }   //if

    else {
        inicialitzacioElements(elements);

        address = 0;
        elements[0] = 16;
        elements[1] = address / 256;
        elements[2] = address - ((address / 256) * 256);
        elements[3] = 0;
        cont = 4;

        for(i = 0; i < MAX_DEN - 1; i++) {
            for (j = 0; j < MAX_NUM - 1; j++) {

                if(cont == MAX_DIM){
                    int k;
                    fprintf(fp, ":%02X", elements[0]);
                    for(k = 1; k < MAX_DIM; k++){
                        fprintf(fp, "%02X", elements[k]);
                    }   //for

                    printf("%X\n", address);
                    fprintf(fp, "%02X\n", calculChecksum(elements));
                    address += ADDRESS;

                    inicialitzacioElements(elements);
                    elements[0] = 16;
                    elements[1] = address / 256;
                    elements[2] = address - ((address / 256) * 256);
                    elements[3] = 0;
                    cont = 4;
                }   //fi

                elements[cont] = binToBCD(realitzaMitjana(j, i));
                cont++;
            }   //for
        }   //for


        fprintf(fp, "%s", ":00000001FF");
        fclose(fp);
    }   //else
    return 0;
}