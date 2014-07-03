#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void     WindowSizeAndDrop  (Parameters *);
float    *InitWinWeights    (int64_t, int32_t);
void     EndWinWeights      (float *);
uint8_t  *FilterSequence    (uint8_t *, Parameters *, float *, uint8_t);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
