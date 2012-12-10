#include "../Include/Simulation.h"

#include <stdio.h>
#include <stdlib.h>

//#define DEBUG_H
//#define MONITOR

/*
 * a useful method for printing a array like a Code.
 */
void print(short* x, int length){
    int i = 0;
    for(i = 0; i < length; i++){
        printf("%d", x[i]);
    }
    printf("\n");
}

/*
 * init message bit randomly
 */
short* initSrc(int length){
    short* src = (short*)malloc(length * sizeof(short));
    int i = 0;
    for(i = 0; i < length; i++){
        double p = (rand() * 1.0) / RAND_MAX;
        if(p < 0.5){
            src[i] = 0;
        } else {
            src[i] = 1;
        }
    }
    return src;
}

int main(){
    
    freopen("output.txt", "w", stdout);
    int i = 0;
    int j = 0;
    
    H srcH;
    input_H(&srcH);
#ifdef DEBUG_H
    printf("srcH(use B)\n");
    printHbyB(&srcH);
    printf("srcH(use A)\n");
    printHbyA(&srcH);
#endif

    H stdH;
    int checkNodeSum = formatH(&srcH, &stdH);
#ifdef DEBUG_H
    printf("stdH(use matrix): %d*%d\n", stdH.m, stdH.n);
    printHbyMatrixH(&stdH);
    printf("stdH(use B):\n");//it is normal that the result of printer by B,A and matrix are different.
    printHbyB(&stdH);
    printf("stdH(use A):\n");
    printHbyA(&stdH);
#endif

    G g;
    formatG(&stdH, checkNodeSum, &g);
#ifdef DEBUG_H
    printf("generate matrix G:%d*%d\n", g.k, g.n);
    printG(&g);
#endif
#ifdef DEBUG_H
    mutiply(&stdH, &g);
#endif
    deleteHMatrix(&stdH);
    
    int times = 0;
    int FER = 0;
    int BER = 0;
    while(FER < 20){
        source src = initSrc(g.k);
#ifdef MONITOR
        printf("message bit:\n");
        print(src, g.k);
#endif
        
        Code ldpcCode;
        encode(&g, src, &ldpcCode);
#ifdef MONITOR
        printf("after encode, code:\n"); 
        print(ldpcCode.x, ldpcCode.length);
#endif

        Code interleavedCode;
        interleave(&ldpcCode, &interleavedCode);
#ifdef MONITOR
        printIP();
        printf("after interleaved:\n");
        print(interleavedCode.x, interleavedCode.length);
#endif

        Marker marker;
        marker.nc = 30;
        marker.nm = 2;
        marker.x = (short*)malloc(marker.nm * sizeof(short));
        marker.x[0] = 0;
        marker.x[1] = 1;
        Code markerCode;
        insertMarker(&interleavedCode, &marker, &markerCode);
#ifdef MONITOR
        printf("insert marker:\n");
        print(markerCode.x, markerCode.length);
#endif
        
        ChannelPara channelPara = {0, 0.011, 0};
        Code receMessage;
        transmit(&markerCode, channelPara, &receMessage);
#ifdef MONITOR
        printf("receive code\n");
        print(receMessage.x, receMessage.length);
        printf("%d %d\n", markerCode.length, receMessage.length); 
#endif
        double12* r = calFinalPro(markerCode.length, &marker, receMessage.x, receMessage.length, &channelPara);
        
        double12* newr = (double12*)malloc(ldpcCode.length * sizeof(double12));
        revInterleave(r, newr);
        
        int err = decode(ldpcCode.x, newr, &stdH);
        if(err > 0){
#ifdef MONITOR
            printf("failed\n");
#endif
            BER += err;
            FER++;
        } else {
#ifdef MONITOR
            printf("succed\n");
#endif
        }
           
        times++;

        free(newr);
        free(r);
        free(receMessage.x);
        free(markerCode.x);
        free(marker.x);
        free(interleavedCode.x);
        free(ldpcCode.x);
        free(src);   
    }
    printf("Times:%d BE:%d FE:%d\n", times, BER, FER);
    printf("BER:%f, FER:%f\n", BER * 1.0 / (stdH.n * times), FER * 1.0 / times);
    
    deleteGMatrix(&g);

    return 0;
}
