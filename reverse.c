#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "reverse.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void IRBlock(U8 *block, U32 nSym, FILE *F)
  {
  U32 k, n = 0;
  U8  stream[nSym];
  
  for(k = nSym ; k-- ; ++n)
    stream[n] = GetCompSym(block[k]);
  fwrite(stream, 1, nSym, F);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

U8 *IRSequence(U8 *fName, Parameters *P, U8 type)
  {
  FILE      *Reader = NULL , *Writter = NULL;
  U64       size, parts, extra;
  clock_t   stop, start;
  U8        *fNameOut, block[BUFFER_REV_SIZE];

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Reversing %s ...\n", type ? "target" : "reference");
    }

  Reader   = Fopen(fName, "r");
  fNameOut = concatenate(fName, ".rev");
  Writter  = Fopen(fNameOut, "w");
  size     = NBytesInFile(Reader);
  parts    = size / BUFFER_REV_SIZE;
  extra    = size % BUFFER_REV_SIZE;
  U8 buffer[extra];

  fseek(Reader, -extra, SEEK_END);
  fread(buffer, 1, extra, Reader);
  IRBlock(buffer, extra, Writter);
  
  size -= extra;
  while(parts--)
    {
    fseek(Reader, size -= BUFFER_REV_SIZE, SEEK_SET);
    fread(block, 1, BUFFER_REV_SIZE, Reader);
    IRBlock(block, BUFFER_REV_SIZE, Writter);
    }

  fclose(Reader);
  fclose(Writter);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Used %g s for reversing %s.\n", ((DB)(stop-start)) 
    / CLOCKS_PER_SEC, type ? "target" : "reference");
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

