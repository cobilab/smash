#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void     WindowSizeAndDrop  (Parameters *);
FL       *InitWinWeights    (I64, I32);
void     EndWinWeights      (FL *);
U8       *FilterSequence    (U8 *, Parameters *, FL *, U8);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
