#ifndef __BASICDEF_H_
#define __BASICDEF_H_

/**
 *  define the const maxsize of the scale of H or G
 *  Earlier, I implements this by macro definition
 *  Now I use the hack enum to implements global definition
 */
enum {
    Max_m = 1000,
    Max_n = 2000,
    Max_wr = 10,
    Max_wc = 5
};

typedef long double double12;

/**
 * identify the check matrix of LDPC: H
 */
typedef struct {
    int n; //column
    int m; //row
    int wc; //column weight    
    int wr; //row weight

    int B[Max_m][Max_wr];    //array B[i] describe the bits which was checked by checknode i
    int A[Max_n][Max_wc];    //arrar A[j] describe the checknodes which check the bit j
    short **Matrix_H;        //two dimension array(0,1) describe H
}H;

//a array that alaways describe a general code
typedef struct {
    short* x;
    int length;
}Code;

//Marker
typedef struct{
    short* x;
    int nm; //maker's length
    int nc; //insert a marker every nc bits
}Marker;

//define the features of a channel
typedef struct{
    double pi;
    double pd;
    double ps;
}ChannelPara;

#endif
