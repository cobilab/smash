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

float *CalcWinWeights(int64_t M, int32_t type)
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

static float Mean(float *ent, int64_t nEnt, int64_t n, int64_t M, float *w)
  {
  int64_t  k;
  float    sum = 0, wSum = 0;

  for(k = -M ; k <= M ; ++k)
    if(n + k >= 0 && n + k < nEnt)
      {
      sum  += w[M+k] * ent[n+k];
      wSum += w[M+k];
      }
  
  return sum / wSum;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *FilterSequence(char *fName, Parameters *P)
  {
  FILE     *Reader  = NULL, *Writter = NULL;
  float    *entries = NULL, *w, *buffer;
  int32_t  wType;
  clock_t  stop, start;
  int64_t  nEntries, n, M, drop, k;
  char     *fNameOut;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Filtering ...\n");
    }

  M        = P->window;
  drop     = P->drop + 1;
  wType    = P->wType;
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

  w = CalcWinWeights(M, wType);

  for(n = 0 ; n != nEntries ; ++n)
    if(n % drop == 0)
      fprintf(Writter, "%"PRIu64"\t%.3f\n", n, Mean(entries, nEntries, n, M, 
      w));

  Free(w);
  Free(entries);
  fclose(Writter);

  if(P->verbose == 1)
    {
    fprintf(stderr, "Done!\n");
    stop = clock();
    fprintf(stderr, "Needed %g s for filtering.\n", ((double) (stop-start)) / 
    CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

