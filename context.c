#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "defs.h"
#include "mem.h"
#include "common.h"
#include "context.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
void FreeCModelCont(CModel *cModel)
  {
  Free(cModel->counters);
  Free(cModel);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void FreeCModelHash(CModel *cModel)
  {
  uint32_t n, k;

  for(k = 0 ; k < HASH_SIZE ; ++k)
    {
    for(n = 0 ; n < cModel->hTable.entrySize[k] ; ++n)
      Free(cModel->hTable.entries[k][n].counters);
    Free(cModel->hTable.entries[k]);
    }

  for(k = 0 ; k < cModel->nSymbols ; ++k)
    Free(cModel->hTable.zeroCounters[k]);
  Free(cModel->hTable.zeroCounters);
  Free(cModel->hTable.entries);
  Free(cModel->hTable.entrySize);
  Free(cModel->counters);
  Free(cModel);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InitHashTable(CModel *cModel)
  {
  cModel->hTable.entries   = (Entry  **) Calloc(HASH_SIZE, sizeof(Entry *));
  cModel->hTable.entrySize = (uint16_t*) Calloc(HASH_SIZE, sizeof(uint16_t));
  cModel->hTable.zeroCounters = (HCCounter **)Calloc(cModel->nSymbols,  
  sizeof(HCCounter *));
  cModel->hTable.nUsedEntries = 0;
  cModel->hTable.nUsedKeys    = 0;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InsertKey(HashTable *hTable, uint32_t hIndex, uint64_t key,
uint32_t nSymbols)
  {
  hTable->entries[hIndex] = (Entry *)Realloc(hTable->entries[hIndex],
  (hTable->entrySize[hIndex] + 1) * sizeof(Entry), sizeof(Entry));

  if(!hTable->entrySize[hIndex])
    hTable->nUsedEntries++;

  hTable->entries[hIndex][hTable->entrySize[hIndex]].counters =
    (HCCounter *)Calloc(nSymbols, sizeof(HCCounter));

  hTable->nUsedKeys++;
  hTable->entries[hIndex][hTable->entrySize[hIndex]].key = key;
  hTable->entrySize[hIndex]++;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

HCCounter *GetHCCounters(HashTable *hTable, uint64_t key)
  {
  uint32_t n, hIndex = (uint32_t)(key % HASH_SIZE);           //The hash index

  for(n = 0 ; n < hTable->entrySize[hIndex] ; n++)
    if(hTable->entries[hIndex][n].key == key)                  // If key found
       return hTable->entries[hIndex][n].counters;
 
  return NULL;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void UpdateHashCounter(CModel *cModel, uint64_t pModelIdx, uint8_t symbol)
  {
  uint32_t n, i, hIndex = (uint32_t)(pModelIdx % HASH_SIZE);
  for(n = 0 ; n < cModel->hTable.entrySize[hIndex] ; n++)
    if(cModel->hTable.entries[hIndex][n].key == pModelIdx)
      {
      if(++cModel->hTable.entries[hIndex][n].counters[symbol] == cModel->maxC)
        for(i = 0 ; i < cModel->nSymbols ; i++)
          cModel->hTable.entries[hIndex][n].counters[i] >>= 1;
      return;
      }
  InsertKey(&cModel->hTable, hIndex, pModelIdx, cModel->nSymbols);
  ++cModel->hTable.entries[hIndex][n].counters[symbol]; 
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

CModel *CreateCModel(uint32_t ctx, uint32_t nSym, uint32_t Den, uint8_t mode,
uint32_t maxC)
  {
  uint32_t  n;
  uint64_t  prod = 1, *tmp;
  CModel    *cModel;
  cModel               = (CModel   *) Calloc(1,   sizeof(CModel  ));
  cModel->nPModels     = (uint64_t)   pow(nSym, ctx);
  cModel->ctxSize      = ctx;
  cModel->nSymbols     = nSym;
  cModel->pModelIdx    = 0;
  cModel->idxIr        = cModel->nPModels - 1;
  cModel->deltaDen     = Den;
  cModel->maxC         = maxC;
  tmp                  = (uint64_t *) Calloc(ctx, sizeof(uint64_t));

  for(n = 0 ; n < ctx ; ++n)
    {
    tmp[n] = prod;
    prod  *= nSym;
    }

  cModel->multiplier = tmp[ctx-1];
  Free(tmp);

  if(mode == 1)
    InitHashTable(cModel);
  else
    cModel->counters = (ACCounter *) Calloc(cModel->nPModels * nSym, 
    sizeof(ACCounter));

  return cModel;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

double InfoSym(uint32_t sum, uint32_t sym)
  { 
  return FLog2((double) sum / sym);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

static HCCounters zeroCounters = {0x00, 0x00, 0x00, 0x00};
static HCCounters auxCounters;

static void InitHashTable(CModel *cModel)
  { 
  cModel->hTable.entries = (Entry **) Calloc(cModel->hTable.size, 
  sizeof(Entry *));
  cModel->hTable.counters = (HCCounters **)Calloc(cModel->hTable.size, 
  sizeof(HCCounters *));
  cModel->hTable.entrySize = (unsigned short *)Calloc(cModel->hTable.size, 
  sizeof(unsigned short));

  cModel->hTable.nUsedEntries = 0;
  cModel->hTable.nUsedKeys = 0;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InitArray(CModel *cModel)
  {
  cModel->array.counters = (ACCounter *) Calloc(cModel->nPModels << 2, 
  sizeof(ACCounter));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InsertKey(HashTable *hTable, unsigned hIndex, uint64_t key)
  {
  hTable->entries[hIndex] = (Entry *) Realloc(hTable->entries[hIndex],
  (hTable->entrySize[hIndex] + 1) * sizeof(Entry), sizeof(Entry));
  
  if(!hTable->entrySize[hIndex])
    hTable->nUsedEntries++;

  hTable->nUsedKeys++;
  hTable->entries[hIndex][hTable->entrySize[hIndex]].key = key;
  hTable->entrySize[hIndex]++;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void InsertCounters(HashTable *hTable, unsigned hIndex, unsigned 
nHCCounters, unsigned k, unsigned smallCounters)
  {
  hTable->counters[hIndex] = (HCCounters *)Realloc(hTable->counters[hIndex], 
  (nHCCounters + 1) * sizeof(HCCounters), sizeof(HCCounters));

  if(k < nHCCounters)
    memmove(hTable->counters[hIndex][k + 1], hTable->counters[hIndex][k],
      (nHCCounters - k) * sizeof(HCCounters));

  hTable->counters[hIndex][k][0] =  smallCounters &  0x03;
  hTable->counters[hIndex][k][1] = (smallCounters & (0x03 << 2)) >> 2;
  hTable->counters[hIndex][k][2] = (smallCounters & (0x03 << 4)) >> 4;
  hTable->counters[hIndex][k][3] = (smallCounters & (0x03 << 6)) >> 6;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static HCCounter *GetHCCounters(HashTable *hTable, uint64_t key)
  {
  unsigned k = 0, n;
  unsigned hIndex = key % hTable->size;

  for(n = 0 ; n < hTable->entrySize[hIndex] ; n++)
    {
    if(hTable->entries[hIndex][n].key == key)
      {
      switch(hTable->entries[hIndex][n].counters)
        {
        case 0:
        return hTable->counters[hIndex][k];

        default:
        auxCounters[0] =  hTable->entries[hIndex][n].counters &  0x03;
        auxCounters[1] = (hTable->entries[hIndex][n].counters & (0x03<<2))>>2;
        auxCounters[2] = (hTable->entries[hIndex][n].counters & (0x03<<4))>>4;
        auxCounters[3] = (hTable->entries[hIndex][n].counters & (0x03<<6))>>6;
        return auxCounters;
        }
      }

    if(hTable->entries[hIndex][n].counters == 0)
      k++;
    }

  return NULL;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

PModel *CreatePModel(unsigned nSymbols)
  {
  PModel *pModel;
  pModel = (PModel *)Malloc(sizeof(PModel));
  pModel->freqs = (unsigned *)Malloc(nSymbols * sizeof(unsigned));

  return pModel;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void UpdateCModelCounter(CModel *cModel, unsigned symbol)
  {
  unsigned n;
  ACCounter *aCounters;
  uint64_t pModelIdx = cModel->pModelIdx;

  if(cModel->mode == HASH_TABLE_MODE)
    {
    unsigned char smallCounter;
    unsigned i, k = 0;
    unsigned nHCCounters; /* The number of HCCounters in this entry */
    unsigned hIndex = pModelIdx % cModel->hTable.size; /* The hash index */

    for(n = 0 ; n < cModel->hTable.entrySize[hIndex] ; n++)
      {
      if(cModel->hTable.entries[hIndex][n].key == pModelIdx)
        {

        // If "counters" is zero, then update the "large" counters.
        if(cModel->hTable.entries[hIndex][n].counters == 0)
          {
          if(++cModel->hTable.counters[hIndex][k][symbol] == 255)
            for(i = 0 ; i < cModel->nSymbols ; i++)
              cModel->hTable.counters[hIndex][k][i] >>= 1;

          return;
          }
        
        smallCounter = (cModel->hTable.entries[hIndex][n].counters >>
          (symbol << 1)) & 0x03;
         // If "counters" is non-zero, then this is at least the
         // second time that this key is generated. Therefore,
         // if the "small" counter of the symbol if full (i.e.,
         // is equal to 3), then the "large" counters have to be
         // inserted into the right position.
        if(smallCounter == 3)
          {
          nHCCounters = k;
          for(i = n + 1 ; i < cModel->hTable.entrySize[hIndex] ; i++)
            if(cModel->hTable.entries[hIndex][i].counters == 0)
              nHCCounters++;

          InsertCounters(&cModel->hTable, hIndex, nHCCounters, k,
          cModel->hTable.entries[hIndex][n].counters);
          cModel->hTable.entries[hIndex][n].counters = 0;
          cModel->hTable.counters[hIndex][k][symbol]++;
          return;
          }
        else // There is still room for incrementing the "small" counter.
          {
          smallCounter++;
          cModel->hTable.entries[hIndex][n].counters &= ~(0x03<<(symbol<<1));
          cModel->hTable.entries[hIndex][n].counters |= 
          (smallCounter<<(symbol<<1));
          return;
          }
        }

      // Keeps counting the number of HCCounters in this entry
      if(!cModel->hTable.entries[hIndex][n].counters)
        k++;
      }

    // If key not found
    InsertKey(&cModel->hTable, hIndex, pModelIdx);
    cModel->hTable.entries[hIndex][cModel->hTable.entrySize[hIndex]-1].
    counters = (0x01 << (symbol << 1));
    }
  else
    {
    aCounters = &cModel->array.counters[pModelIdx * cModel->nSymbols];
    aCounters[symbol]++;
    if(aCounters[symbol] == cModel->maxCount && cModel->maxCount != 0)
      for(n = 0 ; n < cModel->nSymbols ; n++)
        aCounters[n] >>= 1;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CModel *CreateCModel(uint32_t ctx, uint32_t a,  uint32_t hs, uint32_t mc) 
  {
  CModel    *cModel = (CModel *)Calloc(1, sizeof(CModel));
  uint64_t  n, prod = 1;

  if(ctx > 31)
    {
    fprintf(stderr, "Error: context size cannot be greater than 31\n");
    exit(1);
    }

  cModel->nPModels      = (uint64_t) pow(4, ctx);
  cModel->multipliers   = (uint64_t *) Calloc(ctx, sizeof(uint64_t));
  cModel->ctx           = ctx;
  cModel->nSymbols      = 4;
  cModel->deltaNum      = 1;
  cModel->deltaDen      = a;
  cModel->hTable.size   = hs;
  cModel->pModelIdx     = 0;
  cModel->kMinusOneMask = (0x01 << 2 * (ctx - 1)) - 1;

  if(ctx >= HASH_TABLE_BEGIN_CTX)
    {
    cModel->mode     = HASH_TABLE_MODE;
    cModel->maxCount = mc >> 8;
    InitHashTable(cModel);
    }
  else
    {
    cModel->mode     = ARRAY_MODE;
    cModel->maxCount = mc;
    InitArray(cModel);
    }

  for(n = 0 ; n != ctx ; ++n)
    {
    cModel->multipliers[n] = prod;
    prod <<= 2;
    }

  return cModel;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void GetPModelIdx(uint8_t *p, CModel *M)
  {
  M->pModelIdx = ((M->pModelIdx & M->kMinusOneMask) << 2) + (*(p - 1) & 0x03);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ComputePModel(CModel *cModel, PModel *pModel)
  {
  int symbol;
  ACCounter *aCounters;
  HCCounter *hCounters;
  uint64_t pModelIdx = cModel->pModelIdx;

  pModel->sum = 0;
  if(cModel->mode == HASH_TABLE_MODE)
    {
    if(!(hCounters = GetHCCounters(&cModel->hTable, pModelIdx)))
      hCounters = zeroCounters;

    for(symbol = 0 ; symbol < cModel->nSymbols ; symbol++)
      {
      pModel->freqs[symbol] = 1 + cModel->deltaDen * hCounters[symbol];
      pModel->sum += pModel->freqs[symbol];
      }
    }
  else
    {
    aCounters = &cModel->array.counters[pModelIdx * cModel->nSymbols];
    for(symbol = 0 ; symbol < cModel->nSymbols ; symbol++)
      {
      pModel->freqs[symbol] = 1 + cModel->deltaDen * aCounters[symbol];
      pModel->sum += pModel->freqs[symbol];
      }
    }
  }

/*----------------------------------------------------------------------------*/

double PModelSymbolNats(PModel *pModel, unsigned symbol)
  {
  return log((double)pModel->sum / pModel->freqs[symbol]);
  }
