#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "filters.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void WindowSizeAndDrop(Parameters *P)
  {
  if(DEFAULT_WINDOW != -1)
    return;  

  U64 max = P->refSize>P->tarSize?P->refSize:P->tarSize;

  P->subsample = max/DEFAULT_SAMPLE_RATIO;
  if(max < DEFAULT_SAMPLE_RATIO)
    P->subsample = 1;

  P->window = (P->subsample-1)*SUBSAMPLE_RATIO;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FL *InitWinWeights(I64 M, I32 type)
  {
  FL  *w = (FL *) Malloc((2 * M + 1) * sizeof(FL));
  I64 k;

  switch(type)
    {
    case W_HAMMING: for(k = -M ; k <= M ; ++k) w[M+k] = 0.54 + 0.46 * cos((2 *
    M_PI * k) / (2 * M + 1)); break;
    case W_HANN: for(k = -M ; k <= M ; ++k) w[M+k] = 0.5 * (1 + cos((2 * M_PI
    * k) / (2 * M + 1))); break;
    case W_BLACKMAN: for(k = -M ; k <= M ; ++k) w[M+k] = 0.42 + 0.5 * cos((2 *
    M_PI * k) / (2 * M + 1)) + 0.08 * cos((4 * M_PI * k) / (2 * M+1)); break;
    case W_RECTANGULAR: for(k = -M ; k <= M ; ++k) w[M+k] = 1; break;
    }

  return w;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void EndWinWeights(FL *w)
  {
  Free(w);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static FL Mean(FL *ent, I64 nEnt, I64 n, I64 M, FL *w)
  {
  I64 k, s;
  FL  sum = 0, wSum = 0, tmp;

  for(k = -M ; k <= M ; ++k)
    {
    s = n+k;
    if(s >= 0 && s < nEnt)
      {
      sum  += (tmp=w[M+k])*ent[s];
      wSum += tmp;
      }
    }
  
  return sum/wSum;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

U8 *FilterSequence(U8 *fName, Parameters *P, FL *w, U8 remove)
  {
  FILE    *Reader  = NULL, *Writter = NULL;
  FL      *entries = NULL, *buffer;
  I64     nEntries, n, M, drop, k;
  U8      *fNameOut;
  clock_t stop, start;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Filtering ...\n");
    }

  M        = P->window;
  drop     = P->subsample + 1;
  Reader   = Fopen(fName, "rb");
  entries  = (FL *) Malloc(BUFFER_SIZE * sizeof(FL));
  buffer   = (FL *) Malloc(BUFFER_SIZE * sizeof(FL));
  nEntries = 0;

  while((k = fread(buffer, sizeof(FL), BUFFER_SIZE, Reader)))
    {
    for(n = 0 ; n != k ; ++n)
      entries[nEntries++] = buffer[n];
    entries = (FL *) Realloc(entries, (nEntries+k)*sizeof(FL), sizeof(FL)*k);
    }
  fclose(Reader);

  if(P->verbose == 1)
    fprintf(stderr, "Got %"PRIu64" entries from file\n", nEntries);

  fNameOut = concatenate(fName, ".fil");
  Writter  = Fopen(fNameOut, "w");

  for(n = 0 ; n != nEntries ; ++n)
    if(n % drop == 0)
      fprintf(Writter, "%"PRIu64"\t%f\n", n, Mean(entries, nEntries, n, M, w));

  Free(entries);
  fclose(Writter);
  if(remove == 1)
    unlink(fName);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Filtered with %g s.\n", ((DB) (stop-start)) / 
    CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

