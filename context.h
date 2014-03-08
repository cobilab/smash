#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void       FreeCModelCont   (CModel *);
void       FreeCModelHash   (CModel *);
double     InfoSym          (uint32_t, uint32_t);
CModel     *CreateCModel    (uint32_t, uint32_t, uint32_t, uint8_t, uint32_t);
void       UpdateHashCounter(CModel *, uint64_t, uint8_t);
HCCounter  *GetHCCounters   (HashTable *, uint64_t);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
