#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

#define ARRAY_MODE          0
#define HASH_TABLE_MODE     1

typedef unsigned short ACCounter; /* Size of context counters for arrays */
typedef unsigned char  HCCounter; /* Size of context counters for hash tables */
typedef unsigned long long ULL;
typedef HCCounter HCCounters[4];

typedef struct
  {
  uint64_t        key;    /* The key stored in this entry */
  unsigned char   counters;  /* "Small" counters: 2 bits for each one */
  }
Entry;

typedef struct
  {
  unsigned        size;    /* Size of the hash table */
  unsigned short  *entrySize;  /* Number of keys in this entry */
  Entry           **entries;  /* The heads of the hash table lists */
  HCCounters      **counters;  /* The context counters */
  unsigned        nUsedEntries;
  unsigned        nUsedKeys;
  }
HashTable;

typedef struct
  {
  ACCounter       *counters;
  }
Array;

typedef struct
  {
  unsigned   *freqs;
  unsigned   sum;
  }
PModel;

typedef struct
  {
  unsigned   maxCtxSize;  /* Maximum depth of context template */
  unsigned   ctxSize;  /* Current depth of context template */
  unsigned   nSymbols;  /* Number of coding symbols */
  unsigned   nCtxSymbols;/* Number of symbols used for context computation */
  ULL        nPModels;  /* Maximum number of probability models */
  unsigned   deltaNum;  /* Numerator of delta */
  unsigned   deltaDen;  /* Denominator of delta */
  unsigned   maxCount;  /* Counters /= 2 if one counter >= maxCount */
  unsigned   mode;
  HashTable  hTable;
  Array      array;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
void       FreeCModelCont   (CModel *);
void       FreeCModelHash   (CModel *);
double     InfoSym          (uint32_t, uint32_t);
CModel     *CreateCModel    (uint32_t, uint32_t, uint32_t, uint8_t, uint32_t);
void       UpdateHashCounter(CModel *, uint64_t, uint8_t);
HCCounter  *GetHCCounters   (HashTable *, uint64_t);
*/

PModel *CreatePModel(unsigned nSymbols);
void UpdateCModelCounter(CModel *cModel, uint64_t pModelIdx, unsigned symbol);
CModel *CreateCModel(unsigned maxCtxSize, unsigned nSymbols, unsigned 
nCtxSymbols, unsigned deltaNum, unsigned deltaDen, unsigned maxCount,
unsigned hSize);
double FractionOfPModelsUsed(CModel *cModel);
double FractionOfPModelsUsedOnce(CModel *cModel);
void ComputePModel(CModel *cModel, PModel *pModel, uint64_t pModelIdx);
double PModelSymbolNats(PModel *pModel, unsigned symbol);
void HashingStats(CModel *cModel);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
