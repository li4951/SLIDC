#include "../Include/OuterEncoder.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

//#define DEBUG

/**
 * 6_4/96_48 H matrix input
 * B[j][0]: The amount of bits checked by checknode j
 * A[i][0]: The amount of checknodes which checks the bit-i
 */
/*
void input_H(H* srcH) {
    freopen("./Input/96_48.txt", "r", stdin);
    scanf("%d%d", &srcH->n, &srcH->m);
    scanf("%d%d", &srcH->wc, &srcH->wr);

    int rowIndex = 0;
    int colIndex = 0;
    for (colIndex = 0; colIndex < srcH->n; colIndex++) {
        scanf("%d", &srcH->A[colIndex][0]);
    }
    for (rowIndex = 0; rowIndex < srcH->m; rowIndex++) {
        scanf("%d", &srcH->B[rowIndex][0]);
    }

    int k = 0;
    for (colIndex = 0; colIndex < srcH->n; colIndex++) {
        for (k = 1; k <= srcH->A[colIndex][0]; k++) {
            scanf("%d", &srcH->A[colIndex][k]);
            srcH->A[colIndex][k]--;
        }
    }
    for (rowIndex = 0; rowIndex < srcH->m; rowIndex++) {
        for (k = 1; k <= srcH->B[rowIndex][0]; k++) {
            scanf("%d", &srcH->B[rowIndex][k]);
            srcH->B[rowIndex][k]--;
        }
    }
}
*/
/**
 * 504_252 H matrix input
 */

void input_H(H* srcH){
    freopen("./Input/504_252.txt", "r", stdin);
    scanf("%d%d", &srcH->n, &srcH->m);
    scanf("%d%d", &srcH->wc, &srcH->wr);
    int colIndex = 0;
    for(colIndex = 0; colIndex < srcH->n; colIndex++){
        srcH->A[colIndex][0] = 0;
        int j = 0;
        for(j = 1; j <= srcH->wc; j++){
            int temp = 0;
            scanf("%d", &temp);
            if(0 == temp){
                continue;
            }
            srcH->A[colIndex][j] = temp - 1;
            srcH->A[colIndex][0]++;;
        }
    }
    int rowIndex = 0;
    for(rowIndex = 0; rowIndex < srcH->m; rowIndex++){
        srcH->B[rowIndex][0] = 0;
        int j = 0;
        for(j = 1; j <= srcH->wr; j++){
            int temp = 0;
            scanf("%d", &temp);
            if(0 == temp){
                continue;
            }
            srcH->B[rowIndex][j] = temp - 1;
            srcH->B[rowIndex][0]++;
        }
    }
}

/**    
 * print H_matrix by B
 */
void printHbyB(H* h){
    int rowIndex = 0;
    char* row = (char*)malloc(h->n + 1);
    for(rowIndex = 0; rowIndex < h->m; rowIndex++){
        memset(row, '0', h->n);
        int k = 0;
        for(k = 1; k <= h->B[rowIndex][0]; k++){
            int bitOnePosition = h->B[rowIndex][k];
            row[bitOnePosition] = '1';
        }
        row[h->n] = '\0';
        printf("%s\n", row);
    }
    free(row);
    printf("\n");
}

/**
 * print H_matrix by A
 */
void printHbyA(H* h){
    int colIndex = 0;
    char* col = (char*)malloc(h->m + 1);
    for(colIndex = 0; colIndex < h->n; colIndex++){
        memset(col, '0', h->m);
        int k = 0;
        for(k = 1; k <= h->A[colIndex][0]; k++){
            int bitOnePosition = h->A[colIndex][k];
            col[bitOnePosition] = '1';
        }
        col[h->m] = '\0';
        printf("%s\n", col);
    }
    free(col);
    printf("\n");
}

/**
 * print H_matrix by matrix_array
 */
void printHbyMatrixH(H* h){
    if(NULL == h->Matrix_H){
        printf("Marix_H should not be NULL\n");
        return;
    }
    int rowIndex = 0;
    int colIndex = 0;
    for(rowIndex = 0; rowIndex < h->m; rowIndex++){
        for(colIndex = 0; colIndex < h->n; colIndex++){
            printf("%d", h->Matrix_H[rowIndex][colIndex]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * print G_matrix by matrix_array 
 */ 
void printG(G* g) {
    if(NULL == g->Matrix_G){
        printf("Marix_G should not be NULL\n");
        return;
    }
    int rowIndex = 0;
    int colIndex = 0;
    for(rowIndex = 0; rowIndex < g->k; rowIndex++){
        for(colIndex = 0; colIndex < g->n; colIndex++){
            printf("%d", g->Matrix_G[rowIndex][colIndex]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * init matrix_array by B
 */
void initMatrixByB(H* h){
    int rowIndex = 0;
    int colIndex = 0;
    for(rowIndex = 0; rowIndex < h->m; rowIndex++){
        for(colIndex = 0; colIndex < h->n; colIndex++){
            h->Matrix_H[rowIndex][colIndex] = 0;
        }
        int k = 0;
        for(k = 1; k <= h->B[rowIndex][0]; k++){
            int bitOnePosition = h->B[rowIndex][k];
            h->Matrix_H[rowIndex][bitOnePosition] = 1;
        }
    }
}

/**
 * add a row on other row, this operation doesnot affect the essence of the matrix as we studied in linear algebra 
 */
void addRow(short* row1, short* row2, int size){
    int i = 0;
    for(i = 0; i < size; i++){
        row1[i] += row2[i];
        row1[i] %= 2;
    }
}

/**
 * swap two rows which doesnot affect the essence of the matrix as we studied int linear algebra
 */
void swapRow(short* row1, short* row2, int size){
    int i = 0;
    for(i = 0; i < size; i++){
        short t = row1[i];
        row1[i] = row2[i];
        row2[i] = t;
    }
}

/**
 * swap two cols which affect the essence of the matrixm, so it's necessary to modify the A and B of H
 */
void swapCol(H* h, int col1, int col2){
    int i = 0;
    for(i = 0; i < h->m; i++){
        //modify matrix array
        short t = h->Matrix_H[i][col1];
        h->Matrix_H[i][col1] = h->Matrix_H[i][col2];
        h->Matrix_H[i][col2] = t;
        //modify B, attention: may be change the order of B[i]
        int k = 0;
        for(k = 1; k <= h->B[i][0]; k++){
            if(h->B[i][k] == col1){
                h->B[i][k] = col2;
            } else if(h->B[i][k] == col2){
                h->B[i][k] = col1;
            }
        }
    }
    //modify A
    int temp[Max_wc];
    memmove(temp, h->A[col1], Max_wc * sizeof(int));
    memmove(h->A[col1], h->A[col2], Max_wc * sizeof(int));
    memmove(h->A[col2], temp, Max_wc * sizeof(int));
}

/**
 * srcH >> stdH : H(m * n) >> (A[n-k * k], I[n-k])
 * The H(m * n) is alaways not full rank which means some checknode could be deduced by others.
 * Before we get the amount(k) of message bit, we should eliminate the needless checknode row and get the rank m, then k = n - m;
 * So the stdH's m is not equal to that of srcH, and srcH and stdH may be not equal in essence due to some column operation.
 * We encode message with G deduced by stdH, then we can decode with stdH(not srcH).
 */
int formatH(H* srcH, H* stdH){
    memmove(stdH, srcH, sizeof(*srcH));
    stdH->Matrix_H = (short**)malloc(stdH->m * sizeof(short*));
    int i = 0;
    for(i = 0; i < stdH->m; i++){
        stdH->Matrix_H[i] = (short*)malloc(stdH->n * sizeof(short));
    }
    initMatrixByB(stdH);

    //BEGIN: the process that leading diagonal was formated as 1
    int rank = stdH->m;    //initial rank
    int* zeroCol = (int*)malloc(stdH->m*sizeof(int)); //record the col_num which cannot foramted as 1
    int z = 0; //record the size of zeroCol

    //process from low-right to up-left
    int leadRow = 0;    //stepped matrix lead row
    int leadCol = 0;
    for(leadRow = stdH->m - 1, leadCol = stdH->n - 1; leadRow >= 0 && leadCol >= stdH->n - stdH->m; leadRow--, leadCol--){
        //formate the leadRow as leading 1 by row operation, if cannot, do it with col operation
        if(1 != stdH->Matrix_H[leadRow][leadCol]){    //row operation
            int replacedRow = 0;
            for(replacedRow = leadRow - 1; replacedRow >= 0; replacedRow--){
                if(1 == stdH->Matrix_H[replacedRow][leadCol]){
                    swapRow(stdH->Matrix_H[leadRow], stdH->Matrix_H[replacedRow], stdH->n);
                    break;
                }
            }
        }
        if(1 != stdH->Matrix_H[leadRow][leadCol]){ //if needed, do col operation(dangerous)
            int replacedCol = 0;
            //method 2:
            for(replacedCol = 0; replacedCol <= leadCol - 1; replacedCol++){
                if(1 == stdH->Matrix_H[leadRow][replacedCol]){
                    swapCol(stdH, leadCol, replacedCol);
                    break;
                }
            }
          
        }
        if(1 != stdH->Matrix_H[leadRow][leadCol]){ //cannot be formatted as 1 by row or col operation
            rank--;
            zeroCol[z++] = leadCol;//record the col_num(all zero row) which will be replaced in the end
            continue;
        }

        //make sure the leadCol are all 0 except the diagonal
        int t = 0;
        for(t = stdH->m - 1; t >= 0; t--){
            if(t != leadRow && 1 == stdH->Matrix_H[t][leadCol]){
                addRow(stdH->Matrix_H[t], stdH->Matrix_H[leadRow], stdH->n);
            }
        }
    }
    //END: the process that leading diagonal was formated as 1
/*  printf("rank:%d\n", rank):
    printHbyMatrixH(stdH);
*/    
    //put the all-zero row recorded above in front and make sure the col satify UNIT matrix
    int top = 0;
    int left = stdH->n - stdH->m;
    int k = 0;
    for(leadRow = top, leadCol = left; leadRow < stdH->m && leadCol < stdH->n && k < z; leadRow++, leadCol++){
        if(1 == stdH->Matrix_H[leadRow][leadCol]){
            int zcol = zeroCol[k]; 
            int zrow = stdH->m - (stdH->n - zeroCol[k]);
            if(leadCol < zcol){
                swapCol(stdH, leadCol, zcol);
                swapRow(stdH->Matrix_H[leadRow], stdH->Matrix_H[zrow], stdH->n);
            }
            k++;
        }
    }

    //modify the stdH: move the front all-zero line to end line, this operation doesnot affect the essence of H
    for (i = 0; i < stdH->m - z; i++) {
        memcpy(stdH->Matrix_H[i], stdH->Matrix_H[i + z], stdH->n * sizeof(short));
    }
    for (i = stdH->m - z; i < stdH->m; i++) {
        memset(stdH->Matrix_H[i], 0, stdH->n * sizeof(short));
    }

/*  printf("all zero row amount z: %d\n", z);
    printHbyMatrixH(stdH);
*/
    return stdH->m - z;
}

/**
 * deduce G by stdH
 * row:[0,rank) are real checknode, and [rank, stdH->n) are all zero line
 */
void formatG(H* stdH, int rank, G* stdG){
    //get A's translation by A
    short** AT;
    int row_A = stdH->n - rank;
    int col_A = rank;
    AT = (short**)malloc(row_A * sizeof(short*));
    int i = 0;
    int j = 0;
    for(i = 0; i < row_A; i++){
        AT[i] = (short*)malloc(col_A * sizeof(short));
        for(j = 0; j < col_A; j++){
            AT[i][j] = stdH->Matrix_H[j][i];
        }
    }
/*
    printf("printf A's translation:\n");
    for(i = 0; i < row_A; i++){
        for(j = 0; j < col_A; j++){
            printf("%d ", AT[i][j]);
        }
        printf("\n");
    }
    printf("\n");
*/
    //consistute the G
    stdG->k = row_A;
    stdG->n = stdH->n;
    stdG->Matrix_G = (short**)malloc(stdG->k * sizeof(short*));
    for(i = 0; i < row_A; i++){
        stdG->Matrix_G[i] = (short*)malloc(stdG->n*sizeof(short));
        memset(stdG->Matrix_G[i], 0, stdG->n * sizeof(short));
        stdG->Matrix_G[i][i] = 1;
        memcpy(stdG->Matrix_G[i] + stdG->k, AT[i], col_A * sizeof(short));
        free(AT[i]);
    }
    free(AT);
}

/**
 * encode
 * modify 11.30
 */
int encode(G* stdG, source src, Code* ldpcCode){
    
    ldpcCode->length = stdG->n;
    ldpcCode->x = (short*)malloc(stdG->n * sizeof(short));
    memset(ldpcCode->x, 0, stdG->n * sizeof(short));
    int j = 0;
    int i = 0;
    for (j = 0; j < stdG->n; j++) {
        for (i = 0; i < stdG->k; i++) {
            ldpcCode->x[j] += stdG->Matrix_G[i][j] * src[i];
            ldpcCode->x[j] %= 2;
        }
    }
    return ldpcCode->length;
}

/**
 * check H*G==0? if the result of G mutiplied by H(use A & B) is 0, we can use H to decode the code which encode with G. This is kernel.
 * check with A is so troublesome that I didn't implement it. But we can print H by A and B then check the matrixs, in this way can we verify A.
 */
void mutiply(H* h, G* g){
    printf("G*(H's translation, use matrix_array) == 0 ?\n");
    int i = 0;
    int j = 0;
    for(i = 0; i < g->k; i++){
        for(j = 0; j < h->m; j++){
            int sum = 0;
            int k = 0;
            for(k = 0; k < h->n; k++){
                sum ^= (g->Matrix_G[i][k] * h->Matrix_H[j][k]);
            }
            printf("%d", sum);
        }
        printf("\n");
    }
    printf("\n");

    printf("G*(H's translation, use B) == 0 ?\n");
    for(i = 0; i < g->k; i++){
        for(j = 0; j < h->m; j++){
            int sum = 0;
            int k = 0;
            for(k = 1; k <= h->B[j][0]; k++){
                sum ^= g->Matrix_G[i][h->B[j][k]];
            }
            printf("%d", sum);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * release the memory of matrix_array of H
 * only need to release stdH(12/3)
 */
void deleteHMatrix(H* h){
    int i = 0;
    for(i = 0; i < h->m; i++){
        free(h->Matrix_H[i]);
    }
    free(h->Matrix_H);
}

/**
 * release the memory of matrix_array of G
 */
void deleteGMatrix(G* g){
    int i = 0;
    for(i = 0; i < g->k; i++){
        free(g->Matrix_G[i]);
    }
    free(g->Matrix_G);
}

/*
int main(){
    freopen("debug_H.txt", "w", stdout);
    H srcH;
    input_H(&srcH);
#ifdef DEBUG
    printf("srcH(use B)\n");
    printHbyB(&srcH);
    printf("srcH(use A)\n");
    printHbyA(&srcH);
#endif

    H stdH;
    int checkNodeSum = formatH(&srcH, &stdH);
#ifdef DEBUG
    printf("stdH(use matrix): %d*%d\n", stdH.m, stdH.n);
    printHbyMatrixH(&stdH);
    printf("stdH(use B):\n");//it is normal that the result of printer by B,A and matrix are different.
    printHbyB(&stdH);
    printf("stdH(use A):\n");
    printHbyA(&stdH);
#endif

    G stdG;
    formatG(&stdH, checkNodeSum, &stdG);    
    printG(&stdG);

    mutiply(&stdH, &stdG);
    deleteHMatrix(&stdH);
    deleteGMatrix(&stdG);
    return 0;
}
*/

