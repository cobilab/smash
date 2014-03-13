#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "reverse.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *ReverseSequence(char *fName, Parameters *P)
  {
  FILE     *Reader = NULL , *Writter = NULL;
  ULL      pos;
  clock_t  stop, start;
  char     *fNameOut;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Reversing ...\n");
    }

  Reader       = Fopen(fName, "r");
  fNameOut     = concatenate(fName, ".rev");
  Writter      = Fopen(fNameOut, "w");


  //TODO


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

