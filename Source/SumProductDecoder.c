#include "../Include/SumProductDecoder.h"

#include <stdio.h>
#include <math.h>
#include <malloc.h>

/*
void print_M(){
    printf("M matrix:\n");
    int i = 0;
    int j = 0;
    for(i = 0; i < n; i++){
        for(j = 1; j <= A[i][0]; j++){
            printf("%Lf_%d_%Lf ", A[i][j] + 1, i + 1, M[i][j - 1]);
        }
        printf("\n");
    }
}

void print_E(){
    printf("E matrix:\n");
    int i = 0;
    int j = 0;
    for(i = 0; i < m; i++){
        for(j = 1; j <= B[i][0]; j++){
            printf("%d_%Lf_%Lf ", i + 1, B[i][j] + 1, E[i][j - 1]);
        }
        printf("\n");
    }
}
void print_L(){
    printf("L value:\n");
    for(int i = 0; i < n; i++){
        printf("%Lf ", L[i]);
    }
    printf("\n");
}
*/

/**
 * check if H*(z's trans)==0?
 */
int check(short* z, H* parityH){
    int i = 0;
    int j = 0;
    for(i = 0; i < parityH->m; i++){
        int sum = 0;
        for(j = 1; j <= parityH->B[i][0]; j++){
            sum += z[parityH->B[i][j]];
            sum %= 2;
        }
        if(sum != 0){
            return 0;
        }
    }
    return 1;
}

/**
 * iteration process of decoder
 * r is a set of double12 value as a input, if r "can be decode"(could be decode in a constant times and satisfied u*(H's trans)==0). returned.
 * for the performance, the code below is hard to understand. Understanding the following 4 definition is useful to read.  
 * checknode:[0,H.m)
 * bitnode:[0, H.n)
 * bitIndexInCheck:[1, H.B[checknode][0])
 * checkIndexOnBit:[1, H.A[bitnode][0])
 */
void run(double12* r, H* parityH, double12** E, double12** M, double12* L, short* z){
    const int IterMax = 100;//limit for iteration times 

    //step 0: Initialization
    int IterCount = 0; //current times of iteration

    int checknode = 0;
    int bitnode = 0;

    for(bitnode = 0; bitnode < parityH->n; bitnode++){
        int count = 0;
        for(count = 0; count < parityH->A[bitnode][0]; count++){ //attention M[bitnode]: M[bitnode][0]......; but A[bitnode]:A[bitnode][1]...; The beginning is different, but their size are same and saved int A[bitnode][0].
            M[bitnode][count] = r[bitnode];
        }
    }

//    print_M();
    while(1){
        //step 1: check messages
        int i = 0;
        int j = 0;
    
        for(checknode = 0; checknode < parityH->m; checknode++){
            int bitIndexInCheck = 0;
            for(bitIndexInCheck = 1; bitIndexInCheck <= parityH->B[checknode][0]; bitIndexInCheck++){
                double12 product = 1;

                int otherBitIndexInCheck = 0;
                for(otherBitIndexInCheck = 1; otherBitIndexInCheck <= parityH->B[checknode][0]; otherBitIndexInCheck++){
                    if(otherBitIndexInCheck != bitIndexInCheck){
                        int bitnode = parityH->B[checknode][otherBitIndexInCheck];
                        int t = 0;
                        int checkIndexOnBit = 0;
                        for(t = 1; t <= parityH->A[bitnode][0]; t++){
                            if(checknode == parityH->A[bitnode][t]){
                                checkIndexOnBit = --t;
                                break;
                            }
                        }
                        product *= (tanh(M[parityH->B[checknode][otherBitIndexInCheck]][checkIndexOnBit] / 2));
                    }
                }
                if(1 - product > 0 && fabs(1 - product) < inf){ //attention E[checknode]:E[checknode][0]...; but B[checknode]:B[checknode][1]...; The biginning is different, but their size are same saved int B[checknode][0].
                    E[checknode][bitIndexInCheck - 1] = 37.5;
                } else if(1 - product < 0 && fabs(1 - product) < inf) {
                    E[checknode][bitIndexInCheck - 1] = -37.5;
                } else {
                    E[checknode][bitIndexInCheck - 1] = log((1 + product) / (1 - product));
                }
            }
        }
//        print_E();
        //Test if decode success
        for(bitnode = 0; bitnode < parityH->n; bitnode++){
            L[bitnode] = r[bitnode];
        
            int checkIndexOnBit = 0;
            for(checkIndexOnBit = 1; checkIndexOnBit <= parityH->A[bitnode][0]; checkIndexOnBit++){
                int checknode = parityH->A[bitnode][checkIndexOnBit];
                int p = 0;
                int bitIndexInCheck = 0;
                for(p = 1; p <= parityH->B[checknode][0]; p++){
                    if(bitnode == parityH->B[checknode][p]){
                        bitIndexInCheck = --p;
                        break;
                    }
                }
                L[bitnode] += (E[checknode][bitIndexInCheck]);
            }
            if(L[bitnode] <= 0){
                z[bitnode] = 1;
            } else {
                z[bitnode] = 0;
            }
        }
/*        print_L();
        printf("Iteration times %d: ", IterCount);
        for(bitnode = 0; bitnode < parityH->n; bitnode++){
            printf("%d ", z[bitnode]);
        }
        printf("\n");
*/        
        if(check(z, parityH) || IterCount == IterMax){
//            printf("Iteration times: %d\n", IterCount);
            return;
        } else {
            //step 2: bit messages
            for(bitnode = 0; bitnode < parityH->n; bitnode++){
                int checkIndexOnBit = 0;
                for(checkIndexOnBit = 1; checkIndexOnBit <= parityH->A[bitnode][0]; checkIndexOnBit++){
                    double12 sum = 0;
                    int otherCheckIndexOnBit = 0;
                    for(otherCheckIndexOnBit = 1; otherCheckIndexOnBit <= parityH->A[bitnode][0]; otherCheckIndexOnBit++){
                        if(otherCheckIndexOnBit != checkIndexOnBit){
                            int checknode = parityH->A[bitnode][otherCheckIndexOnBit];
                            int p = 0;
                            int bitIndexInCheck = 0;
                            for(p = 1; p <= parityH->B[checknode][0]; p++){
                                if(bitnode == parityH->B[checknode][p]){
                                    bitIndexInCheck = --p;
                                    break;
                                }
                            }
                            sum += E[checknode][bitIndexInCheck];
                            
                        }
                    }
                    M[bitnode][checkIndexOnBit - 1] = sum + r[bitnode];
                }
            }
//            print_M();
            IterCount++;
        }
    }
}

/**
 * the process to ready decoder
 */
int decode(short* x, double12* r, H* parityH){    //mainly use the A and B of H, not care the Matrix_H
    double12** E = (double12**)malloc(parityH->m * sizeof(double12*));
    int i = 0;
    for(i = 0; i < parityH->m; i++){
        E[i] = (double12*)calloc(parityH->n, sizeof(double12));
    }
    double12** M = (double12**)malloc(parityH->n * sizeof(double12*));
    int j = 0;
    for(j = 0; j < parityH->n; j++){
        M[j] = (double12*)calloc(parityH->m, sizeof(double12));
    }
    double12* L = (double12*)calloc(parityH->n, sizeof(double12));
    short* z = (short*)calloc(parityH->n, sizeof(short));

    run(r, parityH, E, M, L, z);

    int flag = 0; //wrong bit count
    for(i = 0; i < parityH->n; i++){
        if(x[i] != z[i]){
            flag++;
        }
    }

/*
    printf("result of decoder:\n");
    for(i = 0; i < parityH->n; i++){
        printf("%d", z[i]);
    }
    printf("\n");
*/

    for(i = 0; i < parityH->m; i++){
        free(E[i]);
    }
    free(E);
    for(j = 0; j < parityH->n; j++){
        free(M[j]);
    }
    free(M);
    free(L);
    free(z);

    return flag;
}
