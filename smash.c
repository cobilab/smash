//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  SMASH    D. Pratas, R. M. Silva, A. J. Pinho, P. J. S. G. Ferreira.     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
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
  #ifndef PROGRESS
  uint64_t  i = 0, size = NBytesInFile(Reader);
  #endif
  clock_t   start = 0, stop = 0;

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
      bits += (uint64_t) (instance = FLog2(pModel->sum / pModel->freqs[sym]));
      outputBuffer[idxPos] = (float) instance;
      if(++idx == BUFFER_SIZE)
        {
        memcpy(symbolBuffer - LEFT_BUFFER_GUARD, symbolBuffer + idx - 
        LEFT_BUFFER_GUARD, LEFT_BUFFER_GUARD);
        idx = 0;
        }
      #ifndef PROGRESS
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
  Free(symbolBuffer-LEFT_BUFFER_GUARD);
  Free(outputBuffer);
  fclose(Reader);
  fclose(Writter);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Needed %g s and %"PRIu64" bytes for compressing "
    "target.\n", ((double) (stop - start)) / CLOCKS_PER_SEC, bits / 8);
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
  #ifndef PROGRESS
  uint64_t  i = 0, size = NBytesInFile(Reader);
  #endif
  clock_t   start = 0, stop = 0;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Building reference model ...\n");
    }

  readerBuffer = (uint8_t *) Calloc(BUFFER_SIZE + 1, sizeof(uint8_t));
  symbolBuffer = (uint8_t *) Calloc(BUFFER_SIZE + LEFT_BUFFER_GUARD + 1, 
  sizeof(uint8_t));
  symbolBuffer += LEFT_BUFFER_GUARD;
  cModel = CreateCModel(P->context, P->alpha, DEFAULT_MAX_COUNT); 
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
      #ifndef PROGRESS
      CalcProgress(size, ++i);
      #endif
      }
  ResetCModel(cModel);
  Free(readerBuffer);
  Free(symbolBuffer-LEFT_BUFFER_GUARD);
  fclose(Reader);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Builded model in %g s.\n", ((double) (stop - 
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
  clock_t  start = 0, stop = 0;

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
        case 65:case 97: writterBuffer[i++]=65; break; // Aa
        case 84:case 116:writterBuffer[i++]=84; break; // Tt
        case 67:case 99: writterBuffer[i++]=67; break; // Cc
        case 71:case 103:writterBuffer[i++]=71; break; // Gg
        #ifdef DRAND48
        case 78:case 119:writterBuffer[i++]=gf[(uint8_t)(drand48()*4)];
        break; // Nn
        #else
        case 78:case 119:writterBuffer[i++]=gf[(uint8_t)(rand()%4)];   
        break; // Nn
        #endif
        default:break;
        }
    fwrite(writterBuffer, 1, i, Writter);
    }
  fclose(Reader);
  fclose(Writter);

  if(P->verbose == 1)
    {
    fprintf(stderr, "Done!\n");
    stop = clock();
    fprintf(stderr, "Done! Needed %g s for randomizing %s.\n", ((double) (stop 
    - start)) / CLOCKS_PER_SEC, type == 0 ? "reference" : "target");
    }

  return fNameOut;
  }


//////////////////////////////////////////////////////////////////////////////
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - M A I N - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int32_t main(int argc, char *argv[])
  {
  char        **p = *&argv, *sRef = NULL, *sTar = NULL, *revRef = NULL, 
              *revTar = NULL, *nameInf = NULL, *nameInfIR = NULL, *nameFil = 
              NULL, *nameFilIR = NULL, *nameSeg = NULL, *nameSegIR = NULL, 
              *nameExt = NULL;
  Parameters  Par;
  Patterns    *patterns = NULL, *patternsIR = NULL, *patternsLB = NULL, 
              *patternsLBIR = NULL;
  Painter     *Paint;
  clock_t     stop = 0, start = 0;
  CModel      *refModel, *refModelIR;
  uint32_t    k, nPatterns, nIRPatterns, n, colorIdx, mult = 0, cip, pt;
  uint64_t    distance;
  int64_t     seed;
  float       *winWeights;
  char        backColor[] = "#ffffff";
  FILE        *PLOT = NULL, *POS = NULL;

  Parameters  *P = &Par;
  if((P->help = ArgsState(DEFAULT_HELP, p, argc, "-h")) == 1 || argc < 2)
    {
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, "Usage: smash <OPTIONS>... [FILE] [FILE]              \n");
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, " -h                  give this help,                 \n");
    fprintf(stderr, " -V                  display version number,         \n");
    fprintf(stderr, " -v                  verbose mode,                   \n");
    fprintf(stderr, " -f                  force (be sure!),               \n");
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, " -c  <context>       context order (DEF: %u),        \n", 
    DEFAULT_CONTEXT);
    fprintf(stderr, " -t  <threshold>     threshold [0.0,2.0] (DEF: %.2g),\n",
    DEFAULT_THRESHOLD);
    fprintf(stderr, " -m  <mSize>         minimum block size (DEF: %u),   \n",
    DEFAULT_MINIMUM);
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, " -i                  show only inverted repeats,     \n");
    fprintf(stderr, " -r  <ratio>         image size ratio (MaxSeq/%u),   \n",
    DEFAULT_SCALE);
    fprintf(stderr, " -a  <alpha>         alpha estimator (DEF: %u),      \n",
    DEFAULT_ALPHA);
    fprintf(stderr, " -s  <seed>          seed for random 'N',            \n");
    fprintf(stderr, " -w  <wSize>         window size,                    \n");
    fprintf(stderr, " -wt <wType>         window type [0|1|2|3] (DEF: %u),\n",
    DEFAULT_WIN_TYPE);
    fprintf(stderr, " -d  <dSize>         sub-sample (DEF: %u),           \n", 
    DEFAULT_SAMPLE_RATIO);
    fprintf(stderr, " -nd                 do not delete temporary files,  \n"); 
    fprintf(stderr, " -wi <width>         sequence width (DEF: %.2g),     \n",
    DEFAULT_WIDTH);
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, " -p  <posFile>       output positions file,          \n");
    fprintf(stderr, " -o  <outFile>       output svg plot file,           \n");
    fprintf(stderr, "                                                     \n");
    fprintf(stderr, " [refFile]           reference file,                 \n");
    fprintf(stderr, " [tarFile]           target file.                  \n\n");
    return EXIT_SUCCESS;
    }

  if(ArgsState(DEFAULT_VERSION, p, argc, "-V")){
    fprintf(stderr, "Smash %u.%u\n"
    "Copyright (C) 2014 University of Aveiro.\nThis is Free software. \nYou "
    "may redistribute copies of it under the terms of the GNU General \n"
    "Public License v2 <http://www.gnu.org/licenses/gpl.html>.\nThere is NO "
    "WARRANTY, to the extent permitted by law.\nAuthors: Diogo Pratas, Raquel "
    "M. Silva, Armando J. Pinho, Paulo J. S. G. Ferreira.\n", RELEASE, VERSION);
    return EXIT_SUCCESS;
    }

  P->verbose   = ArgsState (DEFAULT_VERBOSE,      p, argc, "-v" );
  P->verbose   = Args3State(P->verbose,           p, argc, "-vv");
  P->force     = ArgsState (DEFAULT_FORCE,        p, argc, "-f" );
  P->context   = ArgsNumber(DEFAULT_CONTEXT,      p, argc, "-c" );
  P->alpha     = ArgsNumber(DEFAULT_ALPHA,        p, argc, "-a" );
  P->hash      = DEFAULT_HASH_SIZE;
  P->ir        = ArgsState (DEFAULT_IR,           p, argc, "-i" );
  P->seed      = ArgsNumber(DEFAULT_SEED,         p, argc, "-s" );
  P->threshold = ArgsDouble(DEFAULT_THRESHOLD,    p, argc, "-t" );
  P->window    = ArgsNumber(DEFAULT_WINDOW,       p, argc, "-w" );
  P->wType     = ArgsNumber(DEFAULT_WIN_TYPE,     p, argc, "-wt");
  P->width     = ArgsDouble(DEFAULT_WIDTH,        p, argc, "-wi");
  P->subsample = ArgsNumber(DEFAULT_SAMPLE_RATIO, p, argc, "-d" );
  P->del       = ArgsState (DEFAULT_DELETE,       p, argc, "-nd");
  P->minimum   = ArgsNumber(DEFAULT_MINIMUM,      p, argc, "-m" );
  P->positions = ArgsFiles (                      p, argc, "-p", ".pos");
  P->output    = ArgsFiles (                      p, argc, "-o", ".svg");

  P->refSize   = FopenBytesInFile(argv[argc-2]);
  P->tarSize   = FopenBytesInFile(argv[argc-1]);

  // CALCULATE AUTOMATICALLY RATIO
  P->ratio     = ArgsNumber(MAX(P->refSize, P->tarSize) / 
             DEFAULT_SCALE, p, argc, "-r");
  SetRatio(P->ratio);

  seed = (P->seed == DEFAULT_SEED) ? time(NULL) : P->seed;
  if(P->verbose)
    {
    fprintf(stderr, "Using seed: %u.\n", (uint32_t) seed);
    start = clock();
    }
  Paint        = CreatePainter(GetPoint(P->refSize), GetPoint(P->tarSize), 
                 backColor);
  WindowSizeAndDrop(P);
  winWeights   = InitWinWeights(P->window, P->wType);
  sRef         = RandomNChars(argv[argc-2], seed,              P, REF);
  sTar         = RandomNChars(argv[argc-1], seed += SEED_JUMP, P, TAR);
  revRef       = IRSequence(sRef, P, REF);
  revTar       = IRSequence(sTar, P, TAR);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BUILD TARGET MAP  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  if(!P->ir)
    {
    refModel = LoadReference(sRef, P);
    nameInf  = Compress(sTar, refModel, P);
    nameFil  = FilterSequence(nameInf, P, winWeights);
    nameSeg  = SegmentSequence(nameFil, P);   
    patterns = GetPatterns(nameSeg);
    }
  // INVERTED REPEATS  - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if(P->verbose)
    {
    fprintf(stderr, "---------------------------------------------------\n");
    fprintf(stderr, "Mode: re-running with inverted repeats\n");
    }
  refModelIR = LoadReference(revRef, P);
  if(P->del && !revRef) Unlink(revRef);
  nameInfIR  = Compress(sTar, refModelIR, P);
  nameFilIR  = FilterSequence(nameInfIR, P, winWeights);
  nameSegIR  = SegmentSequence(nameFilIR, P);
  patternsIR = GetPatterns(nameSegIR);
  if(P->verbose)
    fprintf(stderr, "===================================================\n");
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // BUILD REFERENCE MAP FOR EACH TARGET PATTERN - - - - - - - - - - - - - - -
  //
  // OUTPUT HEADER
  PLOT = Fopen(P->output,    "w");
  POS  = Fopen(P->positions, "w");
  Paint->width = P->width;

  PrintHead(PLOT, (2 * DEFAULT_CX) + (((Paint->width + DEFAULT_SPACE) * 2) - 
  DEFAULT_SPACE), Paint->maxSize + EXTRA);
  Rect(PLOT, (2 * DEFAULT_CX) + (((Paint->width + DEFAULT_SPACE) * 2) - 
  DEFAULT_SPACE), Paint->maxSize + EXTRA, 0, 0, backColor);

  RectOval(PLOT, Paint->width, Paint->refSize, Paint->cx, Paint->cy, 
  backColor);

  RectOval(PLOT, Paint->width, Paint->tarSize, Paint->cx, Paint->cy, 
  backColor);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  nPatterns = 0; 
  if(!P->ir)
    {
    for(k = 0 ; k != patterns->nPatterns ; ++k)
      if((distance = patterns->p[k].end - patterns->p[k].init) >= P->minimum)
        ++nPatterns; 
    if(P->verbose && patterns->nPatterns != 0)
      fprintf(stderr, "Found %u valid patterns from %u.\n", nPatterns, 
      patterns->nPatterns);
    }

  nIRPatterns = 0;
  for(k = 0 ; k != patternsIR->nPatterns ; ++k)
    if((distance = patternsIR->p[k].end-patternsIR->p[k].init) >= P->minimum)
      ++nIRPatterns;
  if(P->verbose && patternsIR->nPatterns != 0)
    fprintf(stderr, "Found %u inverted valid patterns from %u.\n", 
    nIRPatterns, patternsIR->nPatterns);

  if(nPatterns + nIRPatterns > 0)
    mult = 255 / (nPatterns + nIRPatterns);
  colorIdx = 0;
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  k = 0;
  if(!P->ir)
    {
    cip = 0;
    for(k = 0 ; k != patterns->nPatterns ; ++k)
      if((distance = patterns->p[k].end-patterns->p[k].init)>= P->minimum)
        {
        if(P->verbose)
          fprintf(stderr, "Running valid pattern %u (id:%u) with size "
          "%"PRIu64"\n", ++cip, k+1, patterns->p[k].end-patterns->p[k].init);
        fprintf(POS, "###TARGET\t%u\t%"PRIu64"\t%"PRIu64"\t0-regular\n", k+1,
        patterns->p[k].init, patterns->p[k].end);

        Rect(PLOT, Paint->width, GetPoint(distance), Paint->cx + 
        DEFAULT_SPACE + DEFAULT_WIDTH, Paint->cy + 
        GetPoint(patterns->p[k].init), GetRgbColor(colorIdx * mult));

        nameExt    = ExtractSubSeq(sTar, P, patterns->p[k].init, 
                     patterns->p[k].end);
        refModel   = LoadReference(nameExt, P); 
        nameInf    = Compress(sRef, refModel, P);
        nameFil    = FilterSequence(nameInf, P, winWeights);
        nameSeg    = SegmentSequence(nameFil, P);
        patternsLB = GetPatterns(nameSeg);

        for(n = 0 ; n < patternsLB->nPatterns ; ++n)
          {
          Rect(PLOT, Paint->width, GetPoint(patternsLB->p[n].end - 
          patternsLB->p[n].init), Paint->cx, Paint->cy + 
          GetPoint(patternsLB->p[n].init), GetRgbColor(colorIdx * mult));

          fprintf(POS, "REFERENCE\t%u\t%"PRIu64"\t%"PRIu64"\t0-regular\n", k+1,
          patternsLB->p[n].init, patternsLB->p[n].end);

          }
        ++colorIdx;
        if(P->verbose)
          fprintf(stderr, "--------------------------------------------------"
          "-\n");
        }
    }
  pt = k;

  // INVERTED REPEATS  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  cip = 0;
  for(k = 0 ; k < patternsIR->nPatterns ; ++k)
    if((distance=patternsIR->p[k].end-patternsIR->p[k].init) >= P->minimum)
      {
      if(P->verbose)
        fprintf(stderr, "Running IR valid pattern %u (id:%u) with size "
        "%"PRIu64"\n", ++cip, k+1, patternsIR->p[k].end - 
        patternsIR->p[k].init);
      fprintf(POS, "###TARGET\t%u\t%"PRIu64"\t%"PRIu64"\t1-inverted\n", k+pt,
      patternsIR->p[k].init, patternsIR->p[k].end);

      RectIR(PLOT, Paint->width, GetPoint(distance), Paint->cx + DEFAULT_SPACE 
      + DEFAULT_WIDTH, Paint->cy + GetPoint(patternsIR->p[k].init), 
      GetRgbColor(colorIdx * mult));
 
      nameExt      = ExtractSubSeq(sTar, P, patternsIR->p[k].init,
                     patternsIR->p[k].end);
      revRef       = IRSequence(nameExt, P, REF); 
      refModel     = LoadReference(revRef, P);
      nameInf      = Compress(sRef, refModel, P);
      nameFil      = FilterSequence(nameInf, P, winWeights);
      nameSeg      = SegmentSequence(nameFil, P);
      patternsLBIR = GetPatterns(nameSeg);

      for(n = 0 ; n < patternsLBIR->nPatterns ; ++n)
        {
        Rect(PLOT, Paint->width, GetPoint(patternsLBIR->p[n].end -
        patternsLBIR->p[n].init), Paint->cx, Paint->cy + 
        GetPoint(patternsLBIR->p[n].init), GetRgbColor(colorIdx * mult));

        fprintf(POS, "REFERENCE\t%u\t%"PRIu64"\t%"PRIu64"\t0-regular\n", k+pt,
        patternsLBIR->p[n].init, patternsLBIR->p[n].end);
        }
      ++colorIdx;
      if(P->verbose)
        fprintf(stderr, "---------------------------------------------------"
        "\n");
      }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  EndWinWeights(winWeights);
  Chromosome(PLOT, Paint->width, Paint->refSize, Paint->cx, Paint->cy);
  Chromosome(PLOT, Paint->width, Paint->tarSize, Paint->cx + DEFAULT_SPACE + 
  DEFAULT_WIDTH,
  Paint->cy);
  PrintFinal(PLOT);
  if(!POS) fclose(POS);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if(P->del)
    {
    if(!sRef)    Unlink(sRef);
    if(!sTar)    Unlink(sTar);
    if(!revRef)  Unlink(revRef);
    if(!revTar)  Unlink(revTar);
    if(!nameExt) Unlink(nameExt);
    }

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
