#include "../Include/InsertMarker.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
/**
 * insert marker to the interleavedCode and become a markerCode
 */
void insertMarker(Code* interleavedCode, Marker* marker, Code* markerCode){
    markerCode->length = interleavedCode->length + interleavedCode->length / marker->nc * marker->nm;
    markerCode->x = (short*)malloc(markerCode->length * sizeof(short));

    int i = 0;
    int k = 0;
    for(i = 0; i < interleavedCode->length; i++){
        markerCode->x[k++] = interleavedCode->x[i];
        if(i != 0 && 0 == (i + 1) % marker->nc){
            memcpy(&markerCode->x[k], marker->x, marker->nm * sizeof(short));
            k += marker->nm;
        }
    }
}

