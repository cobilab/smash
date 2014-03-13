#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include "defs.h"

#define ARRAY_MODE          0
#define HASH_TABLE_MODE     1

typedef unsigned short ACCounter;       // Size of context counters for arrays
typedef unsigned char  HCCounter;  // Size of context counters for hash tables
typedef unsigned long long ULL;
typedef HCCounter HCCounters[4];

typedef struct
  {
  uint64_t        key;                         // The key stored in this entry
  unsigned char   counters;           // "Small" counters: 2 bits for each one
  }
Entry;

typedef struct
  {
  unsigned        size;                              // Size of the hash table
  unsigned short  *entrySize;                  // Number of keys in this entry
  Entry           **entries;              // The heads of the hash table lists
  HCCounters      **counters;                          // The context counters
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
  unsigned        *freqs;
  unsigned        sum;
  }
PModel;

typedef struct
  {
  unsigned        ctx;                    // Current depth of context template
  unsigned        nSymbols;                        // Number of coding symbols
  ULL             nPModels;            // Maximum number of probability models
  unsigned        alphaDen;                            // Denominator of alpha
  unsigned        maxCount;        // Counters /= 2 if one counter >= maxCount
  uint64_t        multiplier;
  uint64_t        pModelIdx;
  unsigned        mode;
  HashTable       hTable;
  Array           array;
  }
CModel;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void         FreeCModel          (CModel *);
//inline void  GetPModelIdx3       (uint8_t *, CModel *);
inline void  GetPModelIdx        (uint8_t *, CModel *);
PModel       *CreatePModel       (unsigned);
void         ResetCModel         (CModel *);
void         UpdateCModelCounter (CModel *, unsigned);
CModel       *CreateCModel       (uint32_t, uint32_t, uint32_t, uint32_t);
void         ComputePModel       (CModel *, PModel *);
double       PModelSymbolNats    (PModel *, unsigned);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
