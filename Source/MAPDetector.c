#include "../Include/MAPDetector.h"
#include "../Include/BasicDef.h"

#include <math.h>
#include <string.h>
#include <float.h>
#include <stdio.h>
#include <malloc.h>

#define MAX_VALUE (LDBL_MAX / 1000)
#define MIN_VALUE (LDBL_MIN * 1000)

static short **visitAlpha;
static short **visitBeta;

double12 F(int xk, int yn, double12 ps){
    return yn == xk ? 1 - ps : ps;
}

int min(int a, int b){
    return a < b ? a : b;
}

int max(int a, int b){
    return a > b ? a : b;
}

void printAlpha(double12** alpha, int T, int R){
    printf("Alpha:\n");
    int k = 0;
    int n = 0;
    for(k = 0; k <= T; k++){
        for(n = 0; n <= R; n++){
            printf("%Lf ", alpha[k][n]);
        }
        printf("\n");
    }
}

void printBeta(double12** beta, int T, int R){
    printf("Beta:\n");
    int k = 0;
    int n = 0;
    for(k = 0; k <= T; k++){
        for(n = 0; n <= R; n++){
            printf("%Lf ", beta[k][n]);
        }
        printf("\n");
    }
}

/**
 *  method 1: iteration
 */
double12 getAlpha(double12** alpha, int n, int k, int T, int R){
    if((k * 2 + R - 2 * T > n) || 2 * k < n || n < 0 || n > R){
        return 0;
    } else if(1 == visitAlpha[k][n]){
        return alpha[k][n];
    } else if ((0 == k) && (0 == n)) {
        alpha[0][0] = 1;
        return alpha[0][0];
    } else {
        printf("Exception in MAPDetector function getAlpha!!!\n");
        return -1;
    }
}

double12 getBeta(double12** beta, int n, int k, int T, int R){
    if((k * 2 + R - 2 * T > n) || 2 * k < n || n < 0 || n > R){
        return 0;
    } else if (1 == visitBeta[k][n]) {
        return beta[k][n];
    } else if ((T == k) && (R == n)) {
        beta[k][n] = 1;
        return beta[k][n];
    } else {
        printf("Exception in MAPDetector function getBeta!!!\n");
        return -1;
    }
}

void calAlpha(double12** alpha, int T, int R, short* y, Marker* marker, ChannelPara* channelPara){
    int k = 0;
    int n = 0;
    int flag = 0;
    for(k = 1; k <= T; k++){
        double12 minValue = LDBL_MAX;
        double12 maxValue = 0;
        for(n = max(0, k * 2 + R - 2 * T); n <= min(2 * k, R); n++){
            double12 sum = 0;
            double12 tempA = getAlpha(alpha, n - 2, k - 1, T, R);
            double12 tempB = getAlpha(alpha, n, k - 1, T, R); 
            double12 tempC = getAlpha(alpha, n - 1, k - 1, T, R);
            sum += (channelPara->pi / 4 * tempA);
            sum += (channelPara->pd * tempB);
            int temp = k % (marker->nc + marker->nm);
            temp == 0 ? temp += (marker->nc + marker->nm) : temp;
            double12 pt = 1 - channelPara->pi - channelPara->pd;
            if(temp > marker->nc){    //marker
                sum = sum + pt * tempC * F(marker->x[temp - marker->nc - 1], y[n], channelPara->ps);
            } else {    //not marker
                sum = sum + pt * tempC * 0.5;
            }
            alpha[k][n] = sum;
            if(alpha[k][n] < minValue && alpha[k][n] > 0){
                minValue = alpha[k][n];
            }
            if(alpha[k][n] > maxValue && alpha[k][n] > 0){
                maxValue = alpha[k][n];
            }
            visitAlpha[k][n] = 1;
        }
        if(minValue < MIN_VALUE && minValue > 0 && !flag){
            for(n = max(0, k * 2 + R - 2 * T); n <= min(2 * k, R); n++){
                alpha[k][n] *= (MAX_VALUE / maxValue);
            }
            flag = 1;
        }
        
    }    
}

void calBeta(double12** beta, int T, int R, short* y, Marker* marker, ChannelPara* channelPara){
    int k = T;
    int n = R;
    int flag = 0;
    for(k = T - 1; k >= 0; k--){
        double12 minValue = LDBL_MAX;
        double12 maxValue = 0;
        for(n = max(0, k * 2 + R - 2 * T); n <= min(2 * k, R); n++){
            double12 sum = 0;
            double12 tempA = getBeta(beta, n + 2, k + 1, T, R);
            double12 tempB = getBeta(beta, n, k + 1, T, R);
            double12 tempC = getBeta(beta, n + 1, k + 1, T, R);
            sum += (channelPara->pi / 4 * tempA);
            sum += (channelPara->pd * tempB);
            int temp = (k + 1) % (marker->nc + marker->nm);
            temp == 0 ? temp += (marker->nc + marker->nm) : temp;
            double12 pt = 1 - channelPara->pi - channelPara->pd;
            if(temp > marker->nc){ //marker
                sum = sum + pt * tempC * F(marker->x[temp - marker->nc - 1], y[n + 1], channelPara->ps);
            } else { //not marker
                sum = sum + pt * tempC * 0.5;
            }
            beta[k][n] = sum;
            if(beta[k][n] < minValue && beta[k][n] > 0){
                minValue = beta[k][n];
            }
            if(beta[k][n] > maxValue && beta[k][n] > 0){
                maxValue = beta[k][n];
            }
            visitBeta[k][n] = 1;
        }

        if(minValue < MIN_VALUE && minValue > 0 && !flag){
            for(n = max(0, k * 2 + R - 2 * T); n <= min(2 * k, R); n++){
                beta[k][n] *= (MAX_VALUE / maxValue);
            }
            flag = 1;
        }
    }    

}

double12* calFinalPro(int T, Marker* marker, short* receive, int R, ChannelPara* channelPara){

    visitAlpha = (short**)malloc((T + 1) * sizeof(short*));
    int temp_i = 0;
    for(temp_i = 0; temp_i <= T; temp_i++){
        visitAlpha[temp_i] = (short*)calloc((R + 1), sizeof(short)); //have init
    }
    visitBeta = (short**)malloc((T + 1) * sizeof(short*));
    for(temp_i = 0; temp_i <= T; temp_i++){
        visitBeta[temp_i] = (short*)calloc((R + 1), sizeof(short)); //have init
    }

    short* y = (short*)calloc((R + 1), sizeof(short));
    memmove(y + 1, receive, R * sizeof(short));

    double12** alpha = (double12**)malloc((T + 1) * sizeof(double12*));
    double12** beta = (double12**)malloc((T + 1) * sizeof(double12*));
    int k = 0;
    for(k = 0; k <= T; k++){
        alpha[k] = (double12*)calloc((R + 1), sizeof(double12)); //have init, althought on double12 type
        beta[k] = (double12*)calloc((R + 1), sizeof(double12));
    }

    calAlpha(alpha, T, R, y, marker, channelPara);
    calBeta(beta, T, R, y, marker, channelPara);

    double12* finalPro = (double12*)malloc((T - T / (marker->nc + marker->nm) * marker->nm) * sizeof(double12));
    int i = 0;
#ifdef DEBUG
    printf("Test finalPro:\n");
#endif
    for(k = 1; k <= T; k++){
        int temp = k % (marker->nc + marker->nm);
        temp == 0 ? temp += (marker->nc + marker->nm) : temp;
        if(temp > marker->nc){    //doesn't need to calculate the marker bit
            continue;
        }
        finalPro[i] = 0.0;
        
        double12 A = 0;
        double12 B = 0;
        double12 C0 = 0;
        double12 C1 = 0;
        int n = 0;
        for(n = 0; n <= min(2 * k, R); n++){
            double12 tempA = getAlpha(alpha, n - 2, k - 1, T, R);
            double12 tempB = getAlpha(alpha, n, k - 1, T, R); 
            double12 tempC = getAlpha(alpha, n - 1, k - 1, T, R);
            beta[k][n] = getBeta(beta, n, k, T, R);

            A += (beta[k][n] > (MAX_VALUE / tempA) ? MAX_VALUE : (tempA * beta[k][n]));
            B += (beta[k][n] > (MAX_VALUE / tempB) ? MAX_VALUE : (tempB * beta[k][n]));
            C0 += ((beta[k][n] > (MAX_VALUE / tempC) ? MAX_VALUE : tempC * beta[k][n]) * F(0, y[n], channelPara->ps));
            C1 += ((beta[k][n] > (MAX_VALUE / tempC) ? MAX_VALUE : tempC * beta[k][n]) * F(1, y[n], channelPara->ps));

        }

        double12 num = (channelPara->pi / 4 * A + channelPara->pd * B + (1 - channelPara->pi - channelPara->pd) * C0);
        double12 den = (channelPara->pi / 4 * A + channelPara->pd * B + (1 - channelPara->pi - channelPara->pd) * C1);

        if(num == 0){
            finalPro[i++] = -37.5;
        } else if(den == 0){
            finalPro[i++] = 37.5;
        } else {
            finalPro[i++] = log(num / den);    
        }
//        printf("%d %Lf %Lf %Lf\n", i, num, den, finalPro[i - 1]);
    }
//    printAlpha(alpha, T, R);
//    printBeta(beta, T, R);

    for(temp_i = 0; temp_i <= T; temp_i++){
        free(visitAlpha[temp_i]);
        free(visitBeta[temp_i]);
        free(alpha[temp_i]);
        free(beta[temp_i]);
    }
    free(visitAlpha);
    free(visitBeta);
    free(alpha);
    free(beta);
    free(y);

    return finalPro;
}

/*
//方式二:递归方式计算 deprecated
double getAlpha(double** alpha, int n, int k, int T, int R, short* y, Marker* marker, ChannelPara* channelPara){
    if(0 == k && 0 == n){
        alpha[0][0] = 1;
        return alpha[0][0];
    } else if((k * 2 + R - 2 * T > n) || 2 * k < n || n < 0 || n > R){
        return 0;
    } else if(1 == visitAlpha[k][n]){
        return alpha[k][n];
    } else {
        double sum = 0;
        double tempA = getAlpha(alpha, n - 2, k - 1, T, R, y, marker, channelPara);
        double tempB = getAlpha(alpha, n, k - 1, T, R, y, marker, channelPara); 
        double tempC = getAlpha(alpha, n - 1, k - 1, T, R, y, marker, channelPara);
        sum += (channelPara->pi / 4 * tempA);
        sum += (channelPara->pd * tempB);
        int temp = k % (marker->nc + marker->nm);
        temp == 0 ? temp += (marker->nc + marker->nm) : temp;
        double pt = 1 - channelPara->pi - channelPara->pd;
        if(temp > marker->nc){    //marker
            sum = sum + pt * tempC * F(marker->x[temp - marker->nc - 1], y[n], channelPara->ps);
        } else {    //not marker
            sum = sum + pt * tempC * 0.5;
        }
        alpha[k][n] = sum;
        visitAlpha[k][n] = 1;
        return alpha[k][n];
    }
}

double getBeta(double** beta, int n, int k, int T, int R, short* y, Marker* marker, ChannelPara* channelPara){
    if(T == k && R == n){
        beta[k][n] = 1;
        return beta[k][n];
    } else if((k * 2 + R - 2 * T > n) || 2 * k < n || n < 0 || n > R){
        return 0;
    } else if(1 == visitBeta[k][n]){
//    } else if(beta[k][n] != 0){
        return beta[k][n];
    } else {
        double sum = 0;
        double tempA = getBeta(beta, n + 2, k + 1, T, R, y, marker, channelPara);
        double tempB = getBeta(beta, n, k + 1, T, R, y, marker, channelPara);
        double tempC = getBeta(beta, n + 1, k + 1, T, R, y, marker, channelPara);
        sum += (channelPara->pi / 4 * tempA);
        sum += (channelPara->pd * tempB);
        int temp = (k + 1) % (marker->nc + marker->nm);
        temp == 0 ? temp += (marker->nc + marker->nm) : temp;
        double pt = 1 - channelPara->pi - channelPara->pd;
        if(temp > marker->nc){ //marker
            sum = sum + pt * tempC * F(marker->x[temp - marker->nc - 1], y[n + 1], channelPara->ps);
        } else { //not marker
            sum = sum + pt * tempC * 0.5;
        }
        beta[k][n] = sum;
        visitBeta[k][n] = 1;
        return beta[k][n];
    }
}

double* calFinalPro(int T, Marker* marker, short* receive, int R, ChannelPara* channelPara){
    //以下这段小代码为我的小case
    visitAlpha = new short* [T + 1];
    int temp_i = 0;
    for(temp_i = 0; temp_i <= T; temp_i++){
        visitAlpha[temp_i] = new short[R + 1];
        memset(visitAlpha[temp_i], 0, sizeof(short) * (R + 1));
    }
    visitBeta = new short* [T + 1];
    for(temp_i = 0; temp_i <= T; temp_i++){
        visitBeta[temp_i] = new short[R + 1];
        memset(visitBeta[temp_i], 0, sizeof(short) * (R + 1));
    }


    short* y = new short[R + 1];

    memmove(y + 1, receive, R * sizeof(short));

    double **alpha = new double* [T + 1];
    double **beta = new double* [T + 1];
    int k = 0;
    for(k = 0; k <= T; k++){
        alpha[k] = new double[R + 1];
        beta[k] = new double[R + 1];
        memset(alpha[k], 0, sizeof(double) * (R + 1));
        memset(beta[k], 0, sizeof(double) * (R + 1));
    }

    double* finalPro = new double[T - T / (marker->nc + marker->nm) * marker->nm];
    int i = 0;
#ifdef DEBUG
    cout << "Test finalPro: " << endl;
#endif
    for(k = 1; k <= T; k++){
        int temp = k % (marker->nc + marker->nm);
        temp == 0 ? temp += (marker->nc + marker->nm) : temp;
        if(temp > marker->nc){    //不计算marker位
            continue;
        }
        finalPro[i] = 0.0;
        
        double A = 0;
        double B = 0;
        double C0 = 0;
        double C1 = 0;
        int n = 0;
        for(n = 0; n <= min(2 * k, R); n++){
            double tempA = getAlpha(alpha, n - 2, k - 1, T, R, y, marker, channelPara);
            double tempB = getAlpha(alpha, n, k - 1, T, R, y, marker, channelPara); 
            double tempC = getAlpha(alpha, n - 1, k - 1, T, R, y, marker, channelPara);
            beta[k][n] = getBeta(beta, n, k, T, R, y, marker, channelPara);

            A = A + tempA * beta[k][n];
            B = B + tempB * beta[k][n];
            C0 = C0 + tempC * beta[k][n] * F(0, y[n], channelPara->ps);
            C1 = C1 + tempC * beta[k][n] * F(1, y[n], channelPara->ps);

        }

        double num = (channelPara->pi / 4 * A + channelPara->pd * B + (1 - channelPara->pi - channelPara->pd) * C0);
        double den = (channelPara->pi / 4 * A + channelPara->pd * B + (1 - channelPara->pi - channelPara->pd) * C1);

        if(num == 0){
            finalPro[i++] = -37.5;
        } else if(den == 0){
            finalPro[i++] = 37.5;
        } else {
            finalPro[i++] = log(num / den);    
        }
        
//        cout << i << endl;
//        cout << num << " " << den << endl;
//        cout << finalPro[i - 1] << endl;

    }
//    printAlpha(alpha, T, R);
//    printBeta(beta, T, R);

    for(temp_i = 0; temp_i <= T; temp_i++){
        delete[] visitAlpha[temp_i];
        delete[] visitBeta[temp_i];
        delete[] alpha[temp_i];
        delete[] beta[temp_i];
    }
    delete[] visitAlpha;
    delete[] visitBeta;
    delete[] alpha;
    delete[] beta;
    delete[] y;
    return finalPro;
}*/
