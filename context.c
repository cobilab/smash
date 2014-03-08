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
