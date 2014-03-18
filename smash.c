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
#include "filters.h"
#include "segment.h"
#include "reverse.h"

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - C O M P R E S S O R - - - - - - - - - - - - -

char *Compress(char *sTar, CModel *cModel, Parameters *P)
  {
  FILE      *Reader  = Fopen(sTar, "r");
  char      *name    = concatenate(sTar, ".inf");
  FILE      *Writter = Fopen(name, "w");
  uint32_t  k, idxPos, instance;
  uint64_t  bits = 0;
  int32_t   idx = 0;
  uint8_t   *readerBuffer, *symbolBuffer, sym;
  PModel    *pModel;
  #ifdef PROGRESS
  uint64_t  i = 0, size = NBytesInFile(Reader);
  #endif
  clock_t   start, stop;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Compressing target sequence ...\n");
    }

  pModel        = CreatePModel(4);
  readerBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + 1, sizeof(uint8_t));
  symbolBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + LEFT_BUFFER_GUARD + 1, sizeof(uint8_t));
  symbolBuffer += LEFT_BUFFER_GUARD;
  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    {
    for(idxPos = 0 ; idxPos < k ; ++idxPos)
      {
      symbolBuffer[idx] = sym = DNASymToNum(readerBuffer[idxPos]);
      GetPModelIdx(symbolBuffer+idx-1, cModel);
      ComputePModel(cModel, pModel);
      bits += (instance = FLog2(pModel->sum / pModel->freqs[sym]));
      fprintf(Writter, "%.*f\n", PRECISION, (float) instance);
      if(++idx == BUFFER_SIZE)
        {
        memcpy(symbolBuffer - LEFT_BUFFER_GUARD, symbolBuffer + idx - 
        LEFT_BUFFER_GUARD, LEFT_BUFFER_GUARD);
        idx = 0;
        }
      #ifdef PROGRESS
      CalcProgress(size, ++i);
      #endif
      }
    }
  FreeCModel(cModel);
  Free(pModel->freqs);
  Free(pModel);
  Free(readerBuffer);
  fclose(Reader);
  fclose(Writter);

  if(P->verbose == 1)
    {
    fprintf(stderr, "Done!                          \n");  // Spaces are valid
    stop = clock();
    fprintf(stderr, "Needed %g s and %"PRIu64" bytes for compressing target."
    "\n", ((double) (stop - start)) / CLOCKS_PER_SEC, bits / 8);
    }

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
  uint64_t  i = 0, size = NBytesInFile(Reader);
  #endif
  clock_t   start, stop;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Building reference model ...\n");
    }

  readerBuffer = (uint8_t *) Calloc(BUFFER_SIZE + 1, sizeof(uint8_t));
  symbolBuffer = (uint8_t *) Calloc(BUFFER_SIZE + LEFT_BUFFER_GUARD + 1, 
  sizeof(uint8_t));
  symbolBuffer += LEFT_BUFFER_GUARD;
  cModel = CreateCModel(P->context, P->alpha, P->hash, DEFAULT_MAX_COUNT); 
  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    for(idxPos = 0 ; idxPos != k ; ++idxPos)
      {
      symbolBuffer[idx] = sym = DNASymToNum(readerBuffer[idxPos]);
      GetPModelIdx(symbolBuffer+idx-1, cModel);
      UpdateCModelCounter(cModel, sym);
      if(++idx == BUFFER_SIZE)
        {
        memcpy(symbolBuffer - LEFT_BUFFER_GUARD, symbolBuffer + 
        idx - LEFT_BUFFER_GUARD, LEFT_BUFFER_GUARD);
        idx = 0;
        }
      #ifdef PROGRESS
      CalcProgress(size, ++i);
      #endif
      }
  ResetCModel(cModel);
  Free(readerBuffer);
  // Free(symbolBuffer); //XXX: WHY CAN'T FREE?
  fclose(Reader);

  if(P->verbose == 1)
    {
    fprintf(stderr, "Done!                          \n");  // Spaces are valid
    stop = clock();
    fprintf(stderr, "Needed %g s for building model.\n", ((double) (stop -
    start)) / CLOCKS_PER_SEC);
    }

  return cModel;
  }
  

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - R A N D O M - - - - - - - - - - - - - - -

char *RandomNChars(char *fName, uint32_t seed, Parameters *P, uint8_t type)
  {
  FILE     *Reader = NULL, *Writter = NULL;
  uint32_t maxIdx, idx, i;
  char     *fNameOut, gf[] = "ACGT", timeMark[MAX_STRING_SIZE];
  uint8_t  readerBuffer[BUFFER_SIZE+1], writterBuffer[BUFFER_SIZE+1];
  clock_t  start, stop;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Randomizing 'N' %s chars ...\n", type == 0 ? "reference"
    : "target");
    }

  srand(seed);
  sprintf(timeMark, ".sys%ux", seed);
  Reader   = Fopen(fName, "r");
  fNameOut = concatenate(fName, timeMark);
  Writter  = Fopen(fNameOut, "w");
  while((maxIdx = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    {
    i = 0;
    for(idx = 0 ; idx < maxIdx ; ++idx)
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
  fclose(Reader);
  fclose(Writter);

  if(P->verbose == 1)
    {
    fprintf(stderr, "Done!\n");
    stop = clock();
    fprintf(stderr, "Needed %g s for randomizing %s.\n", ((double) (stop -
    start)) / CLOCKS_PER_SEC, type == 0 ? "reference" : "target");
    }

  return fNameOut;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[])
  {
  char        **p = *&argv, *sRef, *sTar, *revRef, *revTar, *nameInf, 
              *nameInfIR, *nameFil, *nameFilIR, *nameSeg, *nameSegIR, 
              *nameExt;
  Parameters  Par;
  Patterns    *patterns, *patternsIR, *patternsLB, *patternsLBIR;
  // clock_t     tic, tac, start;
  // double      cpuTimeUsed;
  CModel      *refModel, *refModelIR;
  uint32_t    k;
  int64_t     seed;

  Parameters  *P = &Par;
  if((P->help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2)
    {
    fprintf(stderr, "                                           \n");
    fprintf(stderr, "Usage: smash [OPTIONS]... [FILE] [FILE]    \n");
    fprintf(stderr, "                                           \n");
    fprintf(stderr, " -v                  verbose mode          \n");
    fprintf(stderr, " -f                  force (be sure!)      \n");
    fprintf(stderr, "                                           \n");
    fprintf(stderr, " -c  <context>       context order         \n");
    fprintf(stderr, " -i                  inverted repeats      \n");
    fprintf(stderr, " -a  <alpha>         alpha estimator       \n");
    fprintf(stderr, " -h  <hSize>         hash size             \n");
    fprintf(stderr, "                                           \n");
    fprintf(stderr, " -s  <seed>          seed for random 'N'   \n");
    fprintf(stderr, "                                           \n");
    fprintf(stderr, " -t  <threshold>     threshold [0.0,2.0]   \n");
    fprintf(stderr, " -w  <wSize>         window size           \n");
    fprintf(stderr, " -wt <wType>         window type [0|1|2|3] \n");
    fprintf(stderr, " -d  <dSize>         drop size             \n");
    fprintf(stderr, " -m  <mSize>         minimum block size    \n");
    fprintf(stderr, "                                           \n");
    fprintf(stderr, " <refFile>           reference file        \n");
    fprintf(stderr, " <tarFile>           target file         \n\n");
    return EXIT_SUCCESS;
    }

  P->verbose   = ArgsState (DEFAULT_VERBOSE,   p, argc, "-v" );
  P->force     = ArgsState (DEFAULT_FORCE,     p, argc, "-f" );
  P->context   = ArgsNumber(DEFAULT_CONTEXT,   p, argc, "-c" );
  P->alpha     = ArgsNumber(DEFAULT_ALPHA,     p, argc, "-a" );
  P->hash      = ArgsNumber(DEFAULT_HASH_SIZE, p, argc, "-h" );
  P->seed      = ArgsNumber(DEFAULT_SEED,      p, argc, "-s" );
  P->threshold = ArgsNumber(DEFAULT_THRESHOLD, p, argc, "-t" );
  P->window    = ArgsNumber(DEFAULT_WINDOW,    p, argc, "-w" );
  P->wType     = ArgsNumber(DEFAULT_WIN_TYPE,  p, argc, "-wt");
  P->drop      = ArgsNumber(DEFAULT_DROP,      p, argc, "-d" );
  P->minimum   = ArgsNumber(DEFAULT_MINIMUM,   p, argc, "-m" );

  seed = (P->seed == DEFAULT_SEED) ? time(NULL) : P->seed;
  if(P->verbose)
    fprintf(stderr, "Using seed: %u.\n", (uint32_t) seed);
  P->refSize = 0;
  P->tarSize = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BUILD TARGET MAP  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  sRef       = RandomNChars(argv[argc-2], seed,              P, REF);
  sTar       = RandomNChars(argv[argc-1], seed += SEED_JUMP, P, TAR);
  refModel   = LoadReference(sRef, P);
  nameInf    = Compress(sTar, refModel, P);
  nameFil    = FilterSequence(nameInf, P);
  unlink(nameInf);
  nameSeg    = SegmentSequence(nameFil, P);   
  unlink(nameFil);
  patterns   = GetPatterns(nameSeg);
  unlink(nameSeg);
  //
  // INVERTED REPEATS  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if(P->verbose)
    {
    fprintf(stderr, "---------------------------------------------------\n");
    fprintf(stderr, "Mode: re-running with inverted repeats\n");
    }
  revRef     = IRSequence(sRef, P, REF);
  revTar     = IRSequence(sTar, P, TAR);
  refModelIR = LoadReference(revRef, P);
  nameInfIR  = Compress(sTar, refModelIR, P);    // sTar gives the name to the
  nameFilIR  = FilterSequence(nameInfIR, P);       // and erase previous files
  unlink(nameInfIR);
  nameSegIR  = SegmentSequence(nameFilIR, P);
  unlink(nameFilIR);
  patternsIR = GetPatterns(nameSegIR);
  unlink(nameSegIR);
  if(P->verbose)
    fprintf(stderr, "===================================================\n");
  //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BUILD REFERENCE MAP FOR EACH TARGET PATTERN - - - - - - - - - - - - - - -
  //
  if(P->verbose && patterns->nPatterns != 0)
    fprintf(stderr, "Running %u patterns ...\n", patterns->nPatterns);
  for(k = 0 ; k != patterns->nPatterns ; ++k)
    {
    if(patterns->p[k].end - patterns->p[k].init > P->minimum)
      {
      if(P->verbose)
        fprintf(stderr, "Running pattern %u with size %"PRIu64"\n", k, 
        patterns->p[k].end - patterns->p[k].init);
      nameExt    = ExtractSubSeq(sTar, P, patterns->p[k].init, 
                   patterns->p[k].end);
      refModel   = LoadReference(nameExt, P);
      unlink(nameExt);
      nameInf    = Compress(sRef, refModel, P);
      nameFil    = FilterSequence(nameInf, P);
      unlink(nameInf);
      nameSeg    = SegmentSequence(nameFil, P);
      unlink(nameFil);
      patternsLB = GetPatterns(nameSeg);
      unlink(nameSeg);
      fprintf(stderr, "---------------------------------------------------"
      "\n");
      }
    }
  // INVERTED REPEATS  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  if(P->verbose && patternsIR->nPatterns != 0)
    fprintf(stderr, "Running %u inverted repeats patterns ...\n", 
    patternsIR->nPatterns);
  for(k = 0 ; k != patternsIR->nPatterns ; ++k)
    {
    ;
    }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  unlink(sRef);
  unlink(sTar);
  unlink(revRef);
  unlink(revTar);

  fprintf(stderr, "Done smash algorithmic information map! Enjoy it :)\n");

  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
