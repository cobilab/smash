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
  uint64_t max = (P->refSize > P->tarSize) ? P->refSize : P->tarSize;

  if(DEFAULT_WINDOW != -1)
    return;  

  if(max < WINDOW_RATIO || (max / WINDOW_RATIO < SUBSAMPLE_RATIO)) 
    {
    P->window    = 0;
    P->subsample = 0;
    return;
    }

  P->window    = 150001;  
  P->subsample = P->window / SUBSAMPLE_RATIO;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

float *InitWinWeights(int64_t M, int32_t type)
  {
  float    *w = (float *) Malloc((2 * M + 1) * sizeof(float));
  int64_t  k;

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

void EndWinWeights(float *w)
  {
  Free(w);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static float Mean(float *ent, int64_t nEnt, int64_t n, int64_t M, float *w)
  {
  int64_t  k, s;
  float    sum = 0, wSum = 0, tmp;

  for(k = -M ; k <= M ; ++k)
    {
    s = n + k;
    if(s >= 0 && s < nEnt)
      {
      sum  += (tmp = w[M+k]) * ent[s];
      wSum += tmp;
      }
    }
  
  return sum / wSum;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *FilterSequence(char *fName, Parameters *P, float *w)
  {
  FILE     *Reader  = NULL, *Writter = NULL;
  float    *entries = NULL, *buffer;
  int64_t  nEntries, n, M, drop, k;
  char     *fNameOut;
  clock_t  stop, start;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Filtering ...\n");
    }

  M        = P->window;
  drop     = P->subsample + 1;
  Reader   = Fopen(fName, "rb");
  entries  = (float *) Malloc(BUFFER_SIZE * sizeof(float));
  buffer   = (float *) Malloc(BUFFER_SIZE * sizeof(float));
  nEntries = 0;

  while((k = fread(buffer, sizeof(float), BUFFER_SIZE, Reader)))
    {
    for(n = 0 ; n != k ; ++n)
      entries[nEntries++] = buffer[n];

    entries = (float *) Realloc(entries, (nEntries + k) * sizeof(float), 
    sizeof(float) * k);
    }
  fclose(Reader);

  if(P->verbose == 1)
    fprintf(stderr, "Got %"PRIu64" entries from file\n", nEntries);

  fNameOut = concatenate(fName, ".fil");
  Writter  = Fopen(fNameOut, "w");

  for(n = 0 ; n != nEntries ; ++n)
    if(n % drop == 0)
      fprintf(Writter, "%"PRIu64"\t%.3f\n", n, Mean(entries, nEntries, n, M, 
      w));

  Free(entries);
  fclose(Writter);
  unlink(fName);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Filtered with %g s.\n", ((double) (stop-start)) / 
    CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

