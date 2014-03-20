#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void     WindowSizeAndDrop  (Parameters *);
float    *InitWinWeights    (int64_t, int32_t);
void     EndWinWeights      (float *);
char     *FilterSequence    (char *, Parameters *, float *);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
