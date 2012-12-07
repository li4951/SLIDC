#ifndef __OUTERENCODER_H_
#define __OUTERENCODER_H_

#include "BasicDef.h"

//define message bits waitting to send
typedef short *source;

//define generate matrix
typedef struct{
    int k; //row num
    int n; //col num
    short **Matrix_G;
}G;

void input_H(H* srcH);

void printHbyB(H* h);

void printHbyA(H* h);

void printHbyMatrixH(H* h);

void printG(G* g);

void initMatrixByB(H* aH);

void addRow(short* row1, short* row2, int size);

void swapRow(short* row1, short* row2, int size);

void swapCol(H* aH, int col1, int col2);

int formatH(H* srcH, H* stdH);

void formatG(H* stdH, int checkNodeSum, G* stdG);

int encode(G* stdG, source src, Code* code);

void mutiply(H* h, G* g);

void deleteHMatrix(H* h);

void deleteGMatrix(G* g);

#endif
