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
  Data        *d = (Data *) data;
  FILE        *f;
  char        name[1024];

  return NULL;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[])
  {
  char        **p;
  uint32_t    n;
  Data        P;

  P.help = ArgsState (0, p = *&argv, argc, "-h" );
  if(argc < 2 || Par.F.help == 1)
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
    fprintf(stderr, " -m <mSize>         minimum block size    \n");
    fprintf(stderr, "                                          \n");
    fprintf(stderr, " <refFile>          reference file        \n");
    fprintf(stderr, " <tarFile>          target file         \n\n");
    return EXIT_SUCCESS;
    }

  P.verbose   = ArgsState (0,          p, argc, "-v");
  P.force     = ArgsState (0,          p, argc, "-f");
  P.context   = ArgsNumber(25,         p, argc, "-c");
  P.alpha     = ArgsNumber(1000,       p, argc, "-a");
  P.threshold = ArgsNumber(1.5,        p, argc, "-t");
  P.window    = ArgsNumber(10000,      p, argc, "-w");
  P.minimum   = ArgsNumber(1000000,    p, argc, "-m");

  InRef = Fopen(argv[arvc-2], "r");
  InTar = Fopen(argv[arvc-1], "r");

  //LoadReference();

  //Compress(&P);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
