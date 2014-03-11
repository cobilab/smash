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

char *Compress(char *sTar, CModel *cModel, Parameters *P)
  {
  FILE      *Reader  = Fopen(sTar, "r");
  char      *name    = concatenate(sTar, ".inf");
  FILE      *Writter = Fopen(name, "w");
  uint32_t  k, idxPos;
  int32_t   idx = 0;
  uint8_t   *readerBuffer, *symbolBuffer, sym;
  PModel    *pModel;
  #ifdef PROGRESS
  uint64_t  i = 0;
  #endif

  pModel        = CreatePModel(4);
  readerBuffer  = (uint8_t *) Calloc(BUFFER_SIZE, sizeof(uint8_t));
  symbolBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + LEFT_BUFFER_GUARD,
  sizeof(uint8_t));
  symbolBuffer += LEFT_BUFFER_GUARD;

  if(P->verbose == 1)
    fprintf(stderr, "Compressing target sequence ... \n"); 

  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos != k ; ++idxPos)
      {
      symbolBuffer[idx] = sym = DNASymToNum(readerBuffer[idxPos]);
      GetPModelIdx(symbolBuffer+idx, cModel);
      ComputePModel(cModel, pModel);
      if(++idx == BUFFER_SIZE)
        {
        memcpy(symbolBuffer - LEFT_BUFFER_GUARD, symbolBuffer +
        idx - LEFT_BUFFER_GUARD, LEFT_BUFFER_GUARD);
        idx = 0;
        }
      fprintf(Writter, "%.*f\n", PRECISION, PModelSymbolNats(pModel, sym));
      #ifdef PROGRESS
      CalcProgress(P->tarSize, ++i);
      #endif
      }

  if(P->verbose == 1)
    fprintf(stderr, "Done!                  \n");          // Spaces are valid

  // FREE
  //FreeCModel();
  Free(pModel->freqs);
  Free(pModel);
  Free(readerBuffer);

  return name;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - R E F E R E N C E - - - - - - - - - - - - -

CModel *LoadReference(char *sRef, Parameters *P)
  {
  FILE      *Reader = Fopen(sRef, "r");
  uint32_t  k, idxPos;
  int32_t   idx = 0;
  uint8_t   *readerBuffer, *symbolBuffer, sym;
  CModel    *cModel;
  #ifdef PROGRESS
  uint64_t  i = 0;
  #endif

  readerBuffer  = (uint8_t *) Calloc(BUFFER_SIZE, sizeof(uint8_t));
  symbolBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + LEFT_BUFFER_GUARD, 
  sizeof(uint8_t));
  symbolBuffer += LEFT_BUFFER_GUARD;

  cModel = CreateCModel(P->context, P->alpha, P->hash, DEFAULT_MAX_COUNT); 

  if(P->verbose == 1)
    fprintf(stderr, "Loading reference sequence ... \n");

  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos != k ; ++idxPos)
      {
      symbolBuffer[idx] = sym = DNASymToNum(readerBuffer[idxPos]);
      GetPModelIdx(symbolBuffer+idx, cModel);
      UpdateCModelCounter(cModel, sym);
      if(++idx == BUFFER_SIZE)
        {
        memcpy(symbolBuffer - LEFT_BUFFER_GUARD, symbolBuffer + 
        idx - LEFT_BUFFER_GUARD, LEFT_BUFFER_GUARD);
        idx = 0;
        }
      #ifdef PROGRESS
      CalcProgress(P->refSize, ++i);
      #endif
      }

  if(P->verbose == 1)
    fprintf(stderr, "Done!                  \n");          // Spaces are valid

  Free(readerBuffer);
  //Free(symbolBuffer); //<< WHY CAN'T FREE?
  fclose(Reader);

  return cModel;
  }
  

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - R A N D O M - - - - - - - - - - - - - - -

char *RandomNChars(char *fName, uint32_t seed, Parameters *P, uint8_t type)
  {
  FILE     *Reader = NULL, *Writter = NULL;
  uint32_t maxIdx, idx, i;
  char     readerBuffer[BUFFER_SIZE], writterBuffer[BUFFER_SIZE], *fNameOut,
           gf[] = "ACGT", timeMark[MAX_STRING_SIZE];

  srand(seed += (unsigned) time(NULL));
  sprintf(timeMark, ".sys%ux", seed);

  Reader   = Fopen(fName, "r");
  fNameOut = concatenate(fName, timeMark);
  Writter  = Fopen(fNameOut, "w");

  if(P->verbose == 1)
    fprintf(stderr, "Randomizing 'N' chars ...\n"); 
 
  while((maxIdx = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    {
    i = 0;
    for(idx = 0 ; idx != maxIdx ; ++idx)
      switch(readerBuffer[idx])
        {
        case 65: case 97:  writterBuffer[i++] = 65;             break;   // Aa
        case 84: case 116: writterBuffer[i++] = 84;             break;   // Tt
        case 67: case 99:  writterBuffer[i++] = 67;             break;   // Cc
        case 71: case 103: writterBuffer[i++] = 71;             break;   // Gg
        case 78: case 119: writterBuffer[i++] = gf[rand() % 4]; break;   // Nn
        default: break;
        }
    fwrite(writterBuffer, 1, i, Writter);
    if(type == 0) P->refSize += i; else P->tarSize += i; 
    }

  if(P->verbose == 1)
    fprintf(stderr, "Done!\n");

  fclose(Reader);
  fclose(Writter);
  return fNameOut;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[])
  {
  char        **p = *&argv, *sRef, *sTar, *nameInf;
  Parameters  Par;
  clock_t     tic, tac, start;
  double      cpuTimeUsed;
  CModel      *refModel, *tarModel;

  Parameters  *P = &Par;
  if((P->help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2)
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
    fprintf(stderr, " -h <hSize>         hash size             \n");
    fprintf(stderr, "                                          \n");
    fprintf(stderr, " -t <threshold>     threshold [0.0,2.0]   \n");
    fprintf(stderr, " -w <wSize>         window size           \n");
    fprintf(stderr, " -d <dSize>         drop size             \n");
    fprintf(stderr, " -m <mSize>         minimum block size    \n");
    fprintf(stderr, "                                          \n");
    fprintf(stderr, " <refFile>          reference file        \n");
    fprintf(stderr, " <tarFile>          target file         \n\n");
    return EXIT_SUCCESS;
    }

  P->verbose   = ArgsState (DEFAULT_VERBOSE,   p, argc, "-v");
  P->force     = ArgsState (DEFAULT_FORCE,     p, argc, "-f");
  P->context   = ArgsNumber(DEFAULT_CONTEXT,   p, argc, "-c");
  P->alpha     = ArgsNumber(DEFAULT_ALPHA,     p, argc, "-a");
  P->hash      = ArgsNumber(DEFAULT_HASH_SIZE, p, argc, "-h");
  P->threshold = ArgsNumber(DEFAULT_THRESHOLD, p, argc, "-t");
  P->window    = ArgsNumber(DEFAULT_WINDOW,    p, argc, "-w");
  P->drop      = ArgsNumber(DEFAULT_DROP,      p, argc, "-d");
  P->minimum   = ArgsNumber(DEFAULT_MINIMUM,   p, argc, "-m");

  // 1. RANDOMIZE N CHARS
  if(P->verbose == 1)
    start = clock();

  P->refSize = 0;
  P->tarSize = 0;
  sRef       = RandomNChars(argv[argc-2], 0  , P, 0);
  sTar       = RandomNChars(argv[argc-1], 101, P, 1);

  // 2. EXCLUSIVE CONDITIONAL COMPRESSION
  refModel = LoadReference(sRef, P);
  nameInf  = Compress(sTar, refModel, P);

  if(P->verbose == 1)
    {
    tic = clock();
    cpuTimeUsed = ((double) (tic-start)) / CLOCKS_PER_SEC;
    fprintf(stderr, "Needed %g s for compression.\n", cpuTimeUsed);
    }

//  unlink(nameInf);
//  unlink(sRef);
//  unlink(sTar);

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
