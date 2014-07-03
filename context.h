#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

#define ARRAY_MODE            0
#define HASH_TABLE_MODE       1
#define HASH_TABLE_BEGIN_CTX  15
#define HASH_SIZE             33554467        // first PRIME NUMBER after 2^25
#define MAX_HASH_CTX          28

typedef uint16_t ACCounter;             // Size of context counters for arrays
typedef uint8_t  HCCounter;             // Size of context counters for arrays
typedef uint16_t ENTMAX;                 // Entry size (nKeys for each hIndex)
typedef uint32_t KEYSMAX;                                   // keys index bits
typedef HCCounter HCCounters[4];

typedef struct
  {
  KEYSMAX    key;                              // The key stored in this entry
  uint8_t    counters;                // "Small" counters: 2 bits for each one
  }
Entry;

typedef struct
  {
  ENTMAX     *size;                            // Number of keys in this entry
  Entry      **entries;                   // The heads of the hash table lists
  HCCounters **counters;                               // The context counters
  }
HashTable;

typedef struct
  {
  ACCounter  *counters;
  }
Array;

typedef struct
  {
  uint32_t   *freqs;
  uint32_t   sum;
  }
PModel;

typedef struct
  { 
  uint32_t   ctx;                         // Current depth of context template
  uint32_t   alphaDen;                                 // Denominator of alpha
  uint32_t   maxCount;             // Counters /= 2 if one counter >= maxCount
  uint64_t   multiplier;
  uint64_t   pModelIdx;
  uint64_t   nPModels;                 // Maximum number of probability models
  uint8_t    mode;
  HashTable  hTable;
  Array      array;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void         FreeCModel          (CModel *);
inline void  GetPModelIdx        (uint8_t *, CModel *);
PModel       *CreatePModel       (uint32_t);
void         ResetCModel         (CModel *);
void         UpdateCModelCounter (CModel *, uint32_t);
CModel       *CreateCModel       (uint32_t, uint32_t, uint32_t);
void         ComputePModel       (CModel *, PModel *);
double       PModelSymbolNats    (PModel *, uint32_t);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
