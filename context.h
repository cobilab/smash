#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

#define ARRAY_MODE            0
#define HASH_TABLE_MODE       1
#define HASH_TABLE_BEGIN_CTX  17
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
  U32        *freqs;
  U32        sum;
  }
PModel;

typedef struct
  {
  U32        ctx;                    // Current depth of context template
  U32        alphaDen;                            // Denominator of alpha
  U32        maxCount;        // Counters /= 2 if one counter >= maxCount
  U64        multiplier;
  U64        pModelIdx;
  U64        nPModels;            // Maximum number of probability models
  U8         mode;
  HashTable  hTable;
  Array      array;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void         FreeCModel          (CModel *);
inline void  GetPModelIdx        (U8 *, CModel *);
PModel       *CreatePModel       (U32);
void         ResetCModel         (CModel *);
void         UpdateCModelCounter (CModel *, U32);
CModel       *CreateCModel       (U32, U32, U32);
void         ComputePModel       (CModel *, PModel *);
DB           PModelSymbolNats    (PModel *, U32);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
