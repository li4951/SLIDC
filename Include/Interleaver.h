#ifndef __INTERLEAVER_H_
#define __INTERLEAVER_H_

#include "BasicDef.h"

void initIP(int length);

void interleave(Code* ldpcCode, Code* interleavedCode);

void revInterleave(double12* r, double12* newr);

void printIP();

#endif
