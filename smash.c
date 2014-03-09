#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "mem.h"
#include "defs.h"
#include "common.h"
#include "context.h"

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - C O M P R E S S O R - - - - - - - - - - - - -

void *Compress(void *data)
  {
  //Data        *d = (Data *) data;
  FILE        *f;
  char        name[1024];

  return NULL;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - R A N D O M - - - - - - - - - - - - - - -

char *RandomNonAlphabet(char *fName, uint32_t seed)
  {
  FILE      *Reader = NULL, *Writter = NULL;
  uint32_t  maxIdx, idx;
  char      readerBuffer[BUFFER_SIZE], writterBuffer[BUFFER_SIZE],
            *alphabet = "ATCG", *p, *fNameOut;

  srand(time(NULL) + seed);

  Reader   = Fopen(fName, "r");
  fNameOut = concatenate(fName, ".sys");
  Writter  = Fopen(fNameOut, "w");
  
  while((maxIdx = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    {
    for(idx = 0 ; idx != maxIdx ; ++idx)
      {
      if((p = strchr(alphabet, toupper(readerBuffer[idx]))))
        writterBuffer[idx] = p - alphabet;
      else
        writterBuffer[idx] = alphabet[rand() % 4];
      }
    fwrite(writterBuffer, 1, idx, Writter);
    }

  return fNameOut;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[])
  {
  char        **p = *&argv, *sRef, *sTar;
  uint32_t    n;
  Parameters  P;

  if((P.help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2)
    {
    fprintf(stderr, "                                          \n");
    fprintf(stderr, "Usage: smash [OPTIONS]... [FILE] [FILE]   \n");
    fprintf(stderr, "                                          \n");
    fprintf(stderr, " -v                 verbose mode          \n");
    fprintf(stderr, " -f                 force (be sure!)      \n");
    fprintf(stderr, "                                          \n");
    fprintf(stderr, " -c <context>       context order         \n");
    fprintf(stderr, " -i                 inverted repeats      \n");
    fprintf(stderr, " -a <alpha>         alpha estimator       \n");
    fprintf(stderr, "                                          \n");
    fprintf(stderr, " -t <threshold>     threshold [0.0,2.0]   \n");
    fprintf(stderr, " -a <alpha>         alpha estimator       \n");
    fprintf(stderr, " -w <wSize>         window size           \n");
    fprintf(stderr, " -d <dSize>         drop size             \n");
    fprintf(stderr, " -m <mSize>         minimum block size    \n");
    fprintf(stderr, "                                          \n");
    fprintf(stderr, " <refFile>          reference file        \n");
    fprintf(stderr, " <tarFile>          target file         \n\n");
    return EXIT_SUCCESS;
    }

  P.verbose   = ArgsState (DEFAULT_VERBOSE,   p, argc, "-v");
  P.force     = ArgsState (DEFAULT_FORCE,     p, argc, "-f");
  P.context   = ArgsNumber(DEFAULT_CONTEXT,   p, argc, "-c");
  P.alpha     = ArgsNumber(DEFAULT_ALPHA,     p, argc, "-a");
  P.threshold = ArgsNumber(DEFAULT_THRESHOLD, p, argc, "-t");
  P.window    = ArgsNumber(DEFAULT_WINDOW,    p, argc, "-w");
  P.drop      = ArgsNumber(DEFAULT_DROP,      p, argc, "-d");
  P.minimum   = ArgsNumber(DEFAULT_MINIMUM,   p, argc, "-m");

  sRef = RandomNonAlphabet(argv[argc-2], 42 );
  sTar = RandomNonAlphabet(argv[argc-1], 101);

  //LoadReference();

  //Compress(&P);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
