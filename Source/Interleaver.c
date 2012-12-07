/**
 * The process of interleaving code
 */
#include "../Include/Interleaver.h"
#include "../Include/BasicDef.h"

#include <malloc.h>
#include <stdio.h>

static int* IP; //only be used in this file
static int IP_LENGTH;

/**
 * generate the interleaved permutation
 */
void initIP(int length){
    IP_LENGTH = length;
    IP = (int*)calloc(IP_LENGTH, sizeof(int));
    int* bitset = (int*)calloc(IP_LENGTH, sizeof(int)); //not only allocation, but also initial.

    int i = 0;
    for(i = 0; i < IP_LENGTH; i++){
        int temp = rand() % IP_LENGTH;
        while(1 == bitset[temp]){
            temp = rand() % IP_LENGTH;
        }

        IP[i] = temp;
        bitset[temp] = 1;
    }
}

/**
 * interleave ldpcCode with IP and generate the interleavedCode
 */
void interleave(Code* ldpcCode, Code* interleavedCode){
    initIP(ldpcCode->length);
    interleavedCode->length = ldpcCode->length;
    interleavedCode->x = (short*)malloc(interleavedCode->length * sizeof(short));
    int i = 0;
    for(i = 0; i < ldpcCode->length; i++){
        interleavedCode->x[IP[i]] = ldpcCode->x[i];
    }
}

/**
 * after MAPDetector, releive the order
 */
void revInterleave(double12* r, double12* newr){
    int i = 0;
    for(i = 0; i < IP_LENGTH; i++){
        newr[i] = r[IP[i]];
    }
    free(IP);
}

void printIP(){
    printf("Interleaved Permutation:\n");
    int i = 0;
    for (i = 0; i < IP_LENGTH; i++) {
        printf("%d ", IP[i]);
    }
    printf("\n");
}
