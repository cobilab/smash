#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "reverse.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void IRBlock(char *block, uint32_t nSym, FILE *F)
  {
  uint32_t  k, n = 0;
  char      stream[nSym];
  
  for(k = nSym ; k-- ; ++n)
    stream[n] = GetCompSym(block[k]);
  fwrite(stream, 1, nSym, F);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *IRSequence(char *fName, Parameters *P)
  {
  FILE     *Reader = NULL , *Writter = NULL;
  ULL      size, parts, extra;
  clock_t  stop, start;
  char     *fNameOut, *block, *buffer;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Reversing ...\n");
    }

  Reader   = Fopen(fName, "r");
  fNameOut = concatenate(fName, ".rev");
  Writter  = Fopen(fNameOut, "w");
  size     = NBytesInFile(Reader);
  parts    = size / BUFFER_SIZE;
  extra    = size % BUFFER_SIZE;
  buffer   = (char *) Malloc(extra       * sizeof(char));
  block    = (char *) Malloc(BUFFER_SIZE * sizeof(char));

  fseek(Reader, -extra, SEEK_END);
  fread(buffer, 1, extra, Reader);
  IRBlock(buffer, extra, Writter);
  
  size -= extra;
  while(parts--)
    {
    fseek(Reader, size -= BUFFER_SIZE, SEEK_SET);
    fread(block, 1, BUFFER_SIZE, Reader);
    IRBlock(block, BUFFER_SIZE, Writter);
    }

  Free(block);
  Free(buffer);
  fclose(Reader);
  fclose(Writter);

  if(P->verbose == 1)
    {
    fprintf(stderr, "Done!\n");
    stop = clock();
    fprintf(stderr, "Needed %g s for reversing.\n", ((double) (stop-start)) 
    / CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *IRMemSequence(char *fName, Parameters *P)
  {
  FILE     *Reader = NULL , *Writter = NULL;
  ULL      size, n;
  clock_t  stop, start;
  char     *fNameOut, *sequence, c;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Reversing ...\n");
    }

  Reader   = Fopen(fName, "r");
  fNameOut = concatenate(fName, ".rev");
  Writter  = Fopen(fNameOut, "w");

  size     = NBytesInFile(Reader);
  sequence = (char *) Malloc(size * sizeof(char));

  n = 0;
  while((c = getc(Reader)) != EOF)
    sequence[n++] = c;

  while(n)
    fputc(GetCompSym(sequence[--n]), Writter);

  fclose(Reader);
  fclose(Writter);

  if(P->verbose == 1)
    {
    fprintf(stderr, "Done!\n");
    stop = clock();
    fprintf(stderr, "Needed %g s for reversing.\n", ((double) (stop-start))
    / CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
