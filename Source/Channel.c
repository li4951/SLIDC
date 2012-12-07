#include "../Include/Channel.h"

#include <stdlib.h>
#include <malloc.h>

/*
 * transmit message through BSC
 */
void transmit(Code* transMessage, ChannelPara channelPara, Code* receMessage){
    receMessage->length = transMessage->length * 1.5;
    receMessage->x = (short*)malloc(receMessage->length * sizeof(short));
    int k = 0;
    int i = 0;
    for(i = 0; i < transMessage->length; i++){
        double p = (rand() * 1.0) / RAND_MAX;
        if(p >= 0 && p < channelPara.pi){    //insert
            int t = 2;
            while(t--){
                p = (rand() * 1.0) / RAND_MAX;
                if(p < 0.5){
                    receMessage->x[k++] = 0;
                } else {
                    receMessage->x[k++] = 1;
                }
            }
        } else if(p >= channelPara.pi && p < channelPara.pi + channelPara.pd) { //delete
            continue;
        } else {    //transmit
            p = (rand() * 1.0) / RAND_MAX;
            if(p >= 0 && p < channelPara.ps){    //flip
                receMessage->x[k++] = (transMessage->x[i] + 1) % 2;
            } else {
                receMessage->x[k++] = transMessage->x[i];    //OK
            }
        }
    }
    receMessage->length = k;
}
