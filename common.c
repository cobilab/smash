#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "defs.h"
#include "mem.h"
#include "common.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

float *logTable;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t FLog2(uint64_t i)
  {
  uint32_t n, m, k = 32, o = (i & (i - 1)) ? 1 : 0;
  static const uint64_t sizes[6] = 
  { 0x0000000000000002ull, 0x000000000000000Cull, 0x00000000000000F0ull, 
    0x000000000000FF00ull, 0x00000000FFFF0000ull, 0xFFFFFFFF00000000ull };

  for(n = 6 ; n-- ; )
    {
    o += (m = (i & *(sizes+n)) ? k : 0);
    i >>= m;
    k >>= 1;
    }

  return o;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint64_t NBytesInFile(const char *fn)
  {
  uint64_t size = 0;
  FILE *file = Fopen(fn, "r");

  fseek(file, 0, SEEK_END);
  if((size = ftell(file)) < 1000)
    {
    fprintf(stderr, "Error: input file is very small!\n");
    exit(1);
    }
    
  fclose(file);

  return size;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void FillLogTable(uint32_t nSym, uint32_t alphaDen, uint32_t maxCHigh)
  {
  uint32_t n, maxSize = nSym * maxCHigh * alphaDen;
 
  logTable = (float *) Malloc(maxSize * sizeof(float));
  for(n = 1 ; n != maxSize ; ++n)
    logTable[n] = FLog2(n);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

double SearchLog(uint32_t idx)
  {
  return logTable[idx];
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t GetComp2(uint8_t symbol)
  {
  switch(symbol)
    {
    case 'A': return 'T';
    case 'C': return 'G';
    case 'G': return 'C';
    case 'T': return 'A';
    default:  return symbol;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t GetComp(uint8_t symbol)
  {
  switch(symbol)
    {
    case 0: return 3;
    case 1: return 2;
    case 2: return 1;
    case 3: return 0;
    default:  return symbol;
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FILE *Fopen(const char *path, const char *mode)
  {
  FILE *file = fopen(path, mode);

  if(file == NULL)
    {
    fprintf(stderr, "Error opening: %s (mode %s). Does the file exist?\n", 
    path, mode);
    exit(1);
    }

  return file;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

uint8_t *ReverseStr(uint8_t *str, uint32_t end)
  {
  uint32_t start = 0;

  while(start < end)
    {
    str[start] ^= str[end];
    str[end]   ^= str[start];
    str[start] ^= str[end];
    ++start;
    --end;
    }

  return str;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void SortString(char str[])
  {
  char     tmp;
  int32_t  i, j, length = strlen(str);

  for(i = 0 ; i != length-1 ; ++i)
    for (j = i+1 ; j != length ; ++j)
      if(str[i] > str[j])
        {
        tmp    = str[i];
        str[i] = str[j];
        str[j] = tmp;
        }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *CloneString(char *str)
  {
  char *clone;
  if(str == NULL)
    return NULL;
  strcpy((clone = (char*) Malloc((strlen(str) + 1) * sizeof(char))), str);
  return clone;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *concatenate(char *a, char *b)
  {
  char *base;

  base = (char *) Malloc(strlen(a) + strlen(b) + 1);
  strcpy(base, a);
  strcat(base, b);
  return base;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *RepString(const char *str, const char *old, const char *new)
  {
  size_t sLen = strlen(str) + 1;
  char *cout = 0, *p = 0, *tmp; 

  if(!(p = (cout = (char *) Malloc(sLen * sizeof(char)))))
    return 0;
  while(*str)
    if((*str & 0xc0) != 0x80 && !strncmp(str, old, strlen(old)))
      {
      p   -= (intptr_t) cout;
      tmp  = strcpy(p = (cout = (char *) Realloc(cout, sLen += strlen(new) - 
             strlen(old), strlen(new) - strlen(old))) + (intptr_t) p, new);
      p   += strlen(tmp);
      str += strlen(old);
      }
    else
      *p++ = *str++;
  *p = 0;
  return cout;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint32_t ArgsNumber(uint32_t def, char *arg[], uint32_t n, char *str)
  {
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return atol(arg[n+1]);
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t ArgsState(uint8_t def, char *arg[], uint32_t n, char *str)
  {     
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return def == 0 ? 1 : 0;
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *ArgsString(char *def, char *arg[], uint32_t n, char *str)
  {
  for( ; --n ; )
    if(!strcmp(str, arg[n]))
      return arg[n+1];
  return def;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
void CreaDecFNames(void)
  {
  uint32_t  n;
  char      e1[MAX_FNAME_SIZE], e2[MAX_FNAME_SIZE], **tmp;

  sprintf(e1, ".%s", EXT );
  sprintf(e2, ".%s", DEXT);
  Par.dFileName = (char **) Malloc(Par.nTFiles * sizeof(char *));
  tmp = (char **) Malloc(Par.nTFiles * sizeof(char *));
  for(n = 0 ; n != Par.nTFiles ; ++n)
    {
    tmp[n] = (char *) Malloc(MAX_FNAME_SIZE * sizeof(char));
    strcpy(tmp[n], Par.tFileName[n]);
    Par.dFileName[n] = RepString(tmp[n], e1, e2);
    if(!strstr(tmp[n], e1))
      sprintf(Par.dFileName[n], "%s.%s", Par.dFileName[n], DEXT);
    }
  }
*/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void FAccessWPerm(char *fn)
  {
  if(access(fn, F_OK) != -1)
    {
    fprintf(stderr, "Error: file %s already exists!\n", fn);
    if(access(fn, W_OK) != -1)
      fprintf(stderr, "Note: file %s has write permission.\nTip: to force "
      "writing rerun with \"-f\" option.\nWarning: on forcing, the old (%s) "
      "file will be deleted permanently.\n", fn, fn);
    exit(1);
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
uint32_t ReadFNames(char *def, char *arg[], uint32_t n)
  {
  uint32_t nFiles = 1, k = 0, argLen;
  
  argLen = strlen(arg[n]);
  for( ; k != argLen ; ++k)
    if(arg[n][k] == ':')
      ++nFiles;
  Par.tFileName = (char **) Malloc(nFiles * sizeof(char *));
  Par.tFileName[0] = strtok(arg[n], ":");
  for(k = 1 ; k != nFiles ; ++k)
    Par.tFileName[k] = strtok(NULL, ":");

  return nFiles;
  }
*/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
void LoadReference(void)
  {
  FILE      *f = NULL;
  uint64_t  x = 0, xrc = cRef->nPModels - 1;
  uint32_t  k, idxPos;
  int32_t   idx = 0;
  uint8_t   *seqBuf, *symBuf, *tmp, sym, symC;
  #ifdef PROGRESS
  uint64_t  i = 0;
  #endif

  seqBuf  = (uint8_t *) Calloc(BUF_SIZE,               sizeof(uint8_t));
  symBuf  = (uint8_t *) Calloc(BUF_SIZE + L_GUARD_BUF, sizeof(uint8_t));
  symBuf += L_GUARD_BUF;

  if(Par.F.verbose == 1)
    fprintf(stderr, "Loading reference sequence ... \n");

  f = Fopen(Par.rFileName, "r");
  while((k = fread(seqBuf, 1, BUF_SIZE, f)))
    for(idxPos = 0 ; idxPos != k ; ++idxPos)
      {
      sym = *(seqBuf+idxPos);
      symBuf[idx] = sym = Alpha.alp[sym];
      tmp = &symBuf[idx-1];
      x = ((x - *(tmp-cRef->ctxSize) * cRef->multiplier) * Alpha.nSym) + *tmp;
      UpdateHashCounter(cRef, x, sym);                               // Update 

      if(Par.ri == 1)                                      // Inverted repeats
        {
        xrc = xrc/Alpha.nSym + GetComp(*(tmp+1)) * cRef->multiplier;
        symC = GetComp(*(tmp+1-cRef->ctxSize));
        UpdateHashCounter(cRef, xrc, symC);                          // Update
        }

      if(++idx == BUF_SIZE)
        {
        memcpy(symBuf-L_GUARD_BUF, symBuf+idx-L_GUARD_BUF, L_GUARD_BUF);
        idx = 0;
        }
      #ifdef PROGRESS
      CalcProgress(Alpha.rFileSize, ++i);
      #endif
      }

  if(Par.F.verbose == 1)
    fprintf(stderr, "Done!                  \n");          // Spaces are valid

  Free(seqBuf);
  fclose(f);
  }
*/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void CalcProgress(uint64_t size, uint64_t i)
  {
  if(i % (size / 100) == 0 && size > PROGRESS_MIN)
    fprintf(stderr, "Progress:%3d %%\r", (uint8_t) (i / (size / 100)));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
