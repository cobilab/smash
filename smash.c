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
#include "paint.h"

//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - C O M P R E S S O R - - - - - - - - - - - - -

char *Compress(char *sTar, CModel *cModel, Parameters *P)
  {
  FILE      *Reader  = Fopen(sTar, "r");
  char      *name    = concatenate(sTar, ".inf");
  FILE      *Writter = Fopen(name, "wb");
  uint32_t  k, idxPos, instance;
  uint64_t  bits = 0;
  int32_t   idx = 0;
  uint8_t   *readerBuffer, *symbolBuffer, sym;
  float     *outputBuffer;
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
  outputBuffer  = (float   *) Malloc(BUFFER_SIZE * sizeof(float));
  symbolBuffer  = (uint8_t *) Calloc(BUFFER_SIZE + LEFT_BUFFER_GUARD + 1, 
  sizeof(uint8_t));
  symbolBuffer += LEFT_BUFFER_GUARD;
  while((k = fread(readerBuffer, 1, BUFFER_SIZE, Reader)))
    {
    for(idxPos = 0 ; idxPos < k ; ++idxPos)
      {
      symbolBuffer[idx] = sym = DNASymToNum(readerBuffer[idxPos]);
      GetPModelIdx(symbolBuffer+idx-1, cModel);
      ComputePModel(cModel, pModel);
      bits += (instance = FLog2(pModel->sum / pModel->freqs[sym]));
      outputBuffer[idxPos] = (float) instance;
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
    fwrite(outputBuffer, sizeof(float), k, Writter);
    fflush(Writter);
    }
  
  FreeCModel(cModel);
  Free(pModel->freqs);
  Free(pModel);
  Free(readerBuffer);
  Free(outputBuffer);
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
  // Free(symbolBuffer); //XXX: WHY CAN'T FREE? MEMCOPY?
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
  Painter     *Paint;
  clock_t     stop, start;
  CModel      *refModel, *refModelIR;
  uint32_t    k, nPatterns, n, z;
  uint64_t    distance;
  int64_t     seed;
  float       *winWeights;
  char        backColor[] = "#ffffff";
  FILE        *PLOT = NULL;

  Parameters  *P = &Par;
  if((P->help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2)
    {
    fprintf(stderr, "                                           \n");
    fprintf(stderr, "Usage: smash [OPTIONS]... [FILE] [FILE]    \n");
    fprintf(stderr, "                                           \n");
    fprintf(stderr, " -v                  verbose mode          \n");
    fprintf(stderr, " -vv                 very verbose mode     \n");
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
    fprintf(stderr, " -wi <width>         design sequence width \n");
    fprintf(stderr, "                                           \n");
    fprintf(stderr, " <refFile>           reference file        \n");
    fprintf(stderr, " <tarFile>           target file         \n\n");
    return EXIT_SUCCESS;
    }

  P->verbose   = ArgsState (DEFAULT_VERBOSE,   p, argc, "-v" );
  P->verbose   = Args3State(P->verbose,        p, argc, "-vv");
  P->force     = ArgsState (DEFAULT_FORCE,     p, argc, "-f" );
  P->context   = ArgsNumber(DEFAULT_CONTEXT,   p, argc, "-c" );
  P->alpha     = ArgsNumber(DEFAULT_ALPHA,     p, argc, "-a" );
  P->hash      = ArgsNumber(DEFAULT_HASH_SIZE, p, argc, "-h" );
  P->seed      = ArgsNumber(DEFAULT_SEED,      p, argc, "-s" );
  P->threshold = ArgsDouble(DEFAULT_THRESHOLD, p, argc, "-t" );
  P->window    = ArgsNumber(DEFAULT_WINDOW,    p, argc, "-w" );
  P->wType     = ArgsNumber(DEFAULT_WIN_TYPE,  p, argc, "-wt");
  P->width     = ArgsDouble(DEFAULT_WIDTH,     p, argc, "-wi");
  P->drop      = ArgsNumber(DEFAULT_DROP,      p, argc, "-d" );
  P->minimum   = ArgsNumber(DEFAULT_MINIMUM,   p, argc, "-m" );

  seed = (P->seed == DEFAULT_SEED) ? time(NULL) : P->seed;
  if(P->verbose)
    {
    fprintf(stderr, "Using seed: %u.\n", (uint32_t) seed);
    start = clock();
    }
  P->refSize = FopenBytesInFile(argv[argc-2]);
  P->tarSize = FopenBytesInFile(argv[argc-1]);
  Paint      = CreatePainter(GetPoint(P->refSize), GetPoint(P->tarSize), 
               backColor);
  WindowSizeAndDrop(P);
  winWeights = InitWinWeights(P->window, P->wType);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BUILD TARGET MAP  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  sRef       = RandomNChars(argv[argc-2], seed,              P, REF);
  sTar       = RandomNChars(argv[argc-1], seed += SEED_JUMP, P, TAR);
  refModel   = LoadReference(sRef, P);
  nameInf    = Compress(sTar, refModel, P);
  nameFil    = FilterSequence(nameInf, P, winWeights);
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
  nameFilIR  = FilterSequence(nameInfIR, P, winWeights);
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
  // OUTPUT HEADER
  PLOT = Fopen("plot.svg", "w"); //TODO: NAME: REF-TAR-$RAND.svg
  Paint->width = P->width;

  PrintHead(PLOT, Paint->width, Paint->maxSize);
  Rect(PLOT, 5000.0, 5000.0, 0, 0, backColor); //TODO: DON'T USE FIXED SIZES!
  RectOval(PLOT, Paint->width, Paint->refSize, Paint->cx, Paint->cy, 
  backColor);
  Text(PLOT, Paint->tx, Paint->ty, "S1");
  Paint->tx += Paint->rightShift;
  Paint->cx += Paint->rightShift;

  RectOval(PLOT, Paint->width, Paint->tarSize, Paint->cx, Paint->cy, 
  backColor);

  // TODO: HSV variation
  char Colors[17][8] = {"#005075", //BLUE                   [TOP]
                        "#219691", //GREEN +- BLUE          [TOP]
                        "#CB3A33", //RED DARK               -
                        "#00803E", //GREEN DARK             -
                        "#5DACEC", //BLUE BABY              [1/2 TOP]
                        "#EFDE83", //YELLOW BABY            [1/2 TOP]
                        "#D5D5D3", //SILVER                 [TOP] 
                        "#8F7BB1", //PURPLE BABY            [1/2 TOP]
                        "#3570BB", //BLUE LI                [TOP]
                        "#72417A", //PURPLE ROSE DARK       [TOP]
                        "#B0DDCE", //GREEN GREY BABY        [1/2 TOP]
                        "#BF3F70", //ROSE DARK              [1/3 TOP]
                        "#86A377", //GREEN OLIVER           [1/3 TOP]
                        "#FF7300", //ORANGE                 -
                        "#807068", //BROWN GREY             (SMALL PATTERNS!)
                        "#91B5A8", //GREEN BLUE OLIVER      
                        "#000000"}; //THE REST IS DARK... 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nPatterns = 0; 
  for(k = 0 ; k != patterns->nPatterns ; ++k)
    if((distance = patterns->p[k].end - patterns->p[k].init) >= P->minimum)
      ++nPatterns; 
  if(P->verbose && patterns->nPatterns != 0)
    fprintf(stderr, "Found %u valid patterns from %u.\n", nPatterns, 
    patterns->nPatterns);

  z = 0;
  for(k = 0 ; k != patterns->nPatterns ; ++k)
    if((distance = patterns->p[k].end-patterns->p[k].init) >= P->minimum)
      {
      if(P->verbose)
        fprintf(stderr, "Running pattern %u with size %"PRIu64"\n", k+1, 
        patterns->p[k].end - patterns->p[k].init);

      Rect(PLOT, Paint->width, GetPoint(distance), Paint->cx, Paint->cy + 
      GetPoint(patterns->p[k].init), Colors[z]);

      nameExt    = ExtractSubSeq(sTar, P, patterns->p[k].init, 
                   patterns->p[k].end);
      refModel   = LoadReference(nameExt, P);
      unlink(nameExt);
      nameInf    = Compress(sRef, refModel, P);
      nameFil    = FilterSequence(nameInf, P, winWeights);
      unlink(nameInf);
      nameSeg    = SegmentSequence(nameFil, P);
      unlink(nameFil);
      patternsLB = GetPatterns(nameSeg);
      unlink(nameSeg);
      for(n = 0 ; n != patternsLB->nPatterns ; ++n)
        {
        Rect(PLOT, Paint->width, GetPoint(patternsLB->p[n].end - 
        patternsLB->p[n].init), Paint->cx-Paint->rightShift, Paint->cy + 
        GetPoint(patternsLB->p[n].init), Colors[z]);
        }
      ++z;
      fprintf(stderr, "---------------------------------------------------"
      "\n");
      }
  //
  // INVERTED REPEATS  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  nPatterns = 0;
  for(k = 0 ; k != patternsIR->nPatterns ; ++k)
    if((distance = patternsIR->p[k].end-patternsIR->p[k].init) >= P->minimum)
      ++nPatterns;
  if(P->verbose && patternsIR->nPatterns != 0)
    fprintf(stderr, "Found %u inverted valid patterns from %u.\n", nPatterns,
    patternsIR->nPatterns);

  for(k = 0 ; k != patternsIR->nPatterns ; ++k)
    {
    if((distance = patternsIR->p[k].end-patternsIR->p[k].init) >= P->minimum)
      {
      if(P->verbose)
        fprintf(stderr, "Running pattern %u with size %"PRIu64"\n", k+1,
        patternsIR->p[k].end - patternsIR->p[k].init);

      RectIR(PLOT, Paint->width, GetPoint(distance), Paint->cx, Paint->cy +
      GetPoint(patternsIR->p[k].init), Colors[z]);
 
      nameExt      = ExtractSubSeq(sTar, P, patternsIR->p[k].init,
                     patternsIR->p[k].end);
      revRef       = IRSequence(nameExt, P, REF);
      unlink(nameExt);
      refModel     = LoadReference(revRef, P);
      unlink(nameExt);
      nameInf      = Compress(sRef, refModel, P);
      nameFil      = FilterSequence(nameInf, P, winWeights);
      unlink(nameInf);
      nameSeg      = SegmentSequence(nameFil, P);
      unlink(nameFil);
      patternsLBIR = GetPatterns(nameSeg);
      unlink(nameSeg);
      for(n = 0 ; n != patternsLBIR->nPatterns ; ++n)
        {
        Rect(PLOT, Paint->width, GetPoint(patternsLBIR->p[n].end -
        patternsLBIR->p[n].init), Paint->cx-Paint->rightShift, Paint->cy + 
        GetPoint(patternsLBIR->p[n].init), Colors[z]);
        }
      ++z;
      fprintf(stderr, "---------------------------------------------------"
      "\n");
      }
    }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //                               cx-rightshift
  EndWinWeights(winWeights);
  Chromosome(PLOT, Paint->width, Paint->refSize, Paint->cx -Paint->rightShift, 
  Paint->cy);
  Chromosome(PLOT, Paint->width, Paint->tarSize, Paint->cx, Paint->cy);
  Text(PLOT, Paint->tx, Paint->ty, "S2");
  PrintFinal(PLOT);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  unlink(sRef);
  unlink(sTar);
  unlink(revRef);
  unlink(revTar);

  if(P->verbose)
    {
    stop = clock();
    fprintf(stderr, "Done smash algorithmic information map in %g seconds\n", 
    ((double) (stop-start)) / CLOCKS_PER_SEC);
    fprintf(stderr, "Enjoy it :)\n");
    }
 
  return EXIT_SUCCESS;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
