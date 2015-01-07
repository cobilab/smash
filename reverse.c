#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "reverse.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void IRBlock(uint8_t *i, uint8_t *o, uint32_t nSym, FILE *F)
  {
  uint32_t k, n = 0;
  for(k = nSym ; k-- ; ++n)
    o[n] = GetCompSym(i[k]);
  fwrite(o, 1, nSym, F);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *IRSequence(char *fName, Parameters *P, uint8_t type)
  {
  FILE      *Reader = NULL , *Writter = NULL;
  uint64_t  size;
  clock_t   stop = 0, start = 0;
  char      *fNameOut;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Reversing %s ...\n", type ? "target" : "reference");
    }
 
  Reader   = Fopen(fName, "r");
  fNameOut = concatenate(fName, ".rev");
  Writter  = Fopen(fNameOut, "w");
  size     = NBytesInFile(Reader);

  uint8_t *bIn  = (uint8_t*) malloc((size+1) * sizeof(uint8_t));
  uint8_t *bOut = (uint8_t*) malloc((size+1) * sizeof(uint8_t));

  garbage = fread(bIn, 1, size, Reader);
  IRBlock(bIn, bOut, size, Writter);
  Free(bIn);
  Free(bOut);
  fclose(Reader);
  fclose(Writter);
  
  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Used %g s for reversing %s.\n", ((double)(stop-
    start)) / CLOCKS_PER_SEC, type ? "target" : "reference");
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
