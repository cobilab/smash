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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void PrintArgs(void)
  {
  uint32_t n = 0;
  fprintf(stderr, "Verbose mode ......................... yes\n");
  fprintf(stderr, "Force mode ........................... %s\n", Par.F.force 
  == 0 ? "no" : "yes");
  fprintf(stderr, "Target model:\n");
  fprintf(stderr, "    [+] Context order ................ %u\n", Par.tc);
  fprintf(stderr, "    [+] Max counter .................. %u\n", Par.tm);
  fprintf(stderr, "    [+] Inverted repeats ............. %s\n", Par.ti == 0 
  ? "no" : "yes");
  fprintf(stderr, "Reference model:\n");
  fprintf(stderr, "    [+] Context order ................ %u\n", Par.rc);
  fprintf(stderr, "    [+] Max counter .................. %u\n", Par.rm);
  fprintf(stderr, "    [+] Inverted repeats ............. %s\n", Par.ri == 0 
  ? "no" : "yes");
  fprintf(stderr, "    [+] Alpha denominator ............ %u\n", Par.ra);
  fprintf(stderr, "Block size ........................... %u\n", Par.b);
  fprintf(stderr, "Reference filename ................... %s\n", 
  Par.rFileName);
  fprintf(stderr, "Target files:\n");
  fprintf(stderr, "    [+] Number ....................... %u\n", Par.nTFiles);
  for( ; n != Par.nTFiles ; ++n)
      fprintf(stderr, "    [+] Filename %-2u .................. %s\n", n + 
      1, Par.tFileName[n]);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint64_t CheckAlphabet(char *fName)
  {
  FILE       *f = NULL;
  uint32_t   k, n, idxPos, nSym;
  uint64_t   fileSize = 0;
  char       tmp1[MAX_FNAME_SIZE], tmp2[MAX_FNAME_SIZE], buf[BUF_SIZE],
             norAlp[NSIGNF], alp[NSIGNF], mask[NSIGNF];
 
  (void) alp; 
  f = Fopen(fName, "r");
  while((k = fread(buf, 1, BUF_SIZE, f)))
    for(idxPos = 0 ; idxPos < k ; ++idxPos)
      {
      mask[(uint8_t) *(buf+idxPos)] = 1;
      ++fileSize;
      }
  nSym = 0;
  for(n = MAXSIZEA ; --n ; )                                 // Build alphabet
    if(mask[n] == 1)
      {
      norAlp[nSym] = n;
      alp[n] = nSym++;                       // Don't change to pre-increment!
      }
    else
      alp[n] = INVALID_S;
  fclose(f);
  
  for(n = 0 ; n != nSym ; ++n)
    tmp1[n] = norAlp[n];
  tmp1[n] = '\0';

  for(n = nSym ; n-- ; )
    tmp2[n] = norAlp[n];
  tmp2[nSym] = '\0';

  if(strcmp(tmp1,tmp2))
    {
    fprintf(stderr, "Error: alphabets from reference (%s) and target (%s) "
    "differ!\nFiltering in Linux example: cat targetFile | grep -v \">\""
    " | tr -d -c \"%s\" > newFile\n", tmp1, tmp2, tmp2);
    exit(1);
    }

  return fileSize;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void BuildAlphabet(void)
  {
  FILE      *f = NULL;
  uint32_t  k, n, idxPos;
  uint8_t   *buf, c;

  buf          = (uint8_t *) Calloc(BUF_SIZE, sizeof(uint8_t)); 
  Alpha.norAlp = (uint8_t *) Calloc(NSIGNF,   sizeof(uint8_t));
  Alpha.alp    = (uint8_t *) Calloc(NSIGNF,   sizeof(uint8_t));
  Alpha.mask   = (uint8_t *) Calloc(NSIGNF,   sizeof(uint8_t));

  Alpha.rCheckSum = 0;
  Alpha.rFileSize = 0;
  f = Fopen(Par.rFileName, "r");
  while((k = fread(buf, 1, BUF_SIZE, f)))
    for(idxPos = 0 ; idxPos != k ; ++idxPos)
      {
      c = (uint8_t) buf[idxPos];
      Alpha.mask[c]    = 1;
      Alpha.rCheckSum += c;
      ++Alpha.rFileSize;
      }

  Alpha.nSym = 0;
  for(n = MAXSIZEA ; --n ; )                                 // Build alphabet
    if(Alpha.mask[n] == 1)
      {
      Alpha.norAlp[Alpha.nSym] = n;
      Alpha.alp[n] = Alpha.nSym++;           // Don't change to pre-increment!
      }
    else
      Alpha.alp[n] = INVALID_S; 

  if(Par.F.verbose == 1)
    {
    fprintf(stderr, "Reference sequence properties:\n");
    fprintf(stderr, "    [+] Size ......................... %"PRIu64"\n", 
    Alpha.rFileSize);
    fprintf(stderr, "    [+] Alphabet number .............. %u\n", 
    Alpha.nSym);
    fprintf(stderr, "    [+] Symbols ...................... ");
    for(n = 0 ; n != Alpha.nSym ; ++n)
      Alpha.norAlp[n] != '\n' ? fprintf(stderr, "%c", Alpha.norAlp[n]) :
      fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    [+] Checksum ..................... %"PRIu64"\n", 
    Alpha.rCheckSum);
    }

  Free(buf);
  fclose(f);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void CmpCheckSum(uint64_t cs)
  {
  if(Alpha.rCheckSum != cs)
    { 
    fprintf(stderr, "Error: invalid reference file!\n"
    "Compression reference checksum ................. %"PRIu64"\n"
    "Decompression reference checksum ............... %"PRIu64"\n",
    cs, Alpha.rCheckSum);
    fprintf(stderr, "Tip: rerun with correct reference file\n");
    exit(1);
    }
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void CmpInputError(uint32_t n)
  {
  fprintf(stderr, "%s: unmatching compression/decompression parameters in "
  "reference file (%s)!\n", Par.F.force == 1 ? "Warning" : "Error", 
  Par.rFileName);
  if(Par.F.verbose == 1)
    fprintf(stderr, "Tip: perhaps file \"%s\" has been used in other "
    "compression setup.\n", Par.tFileName[n]);
  if(Par.F.force == 1)
    fprintf(stderr, "Action: file \"%s\" will be ignored from decompression."
    "\n", Par.tFileName[n]);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline void CalcProgress(uint64_t size, uint64_t i)
  {
  if(i % (size / 100) == 0 && size > PROGRESS_MIN)
    fprintf(stderr, "Progress:%3d %%\r", (uint8_t) (i / (size / 100)));
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t *CheckReference(void)
  {
  ac_decoder acd;
  ac_model   acm;
  uint64_t   markC;
  uint32_t   n, rc, rm, ri, ra;
  uint8_t    *run, ok = 0;

  if(Par.F.verbose == 1)
    fprintf(stderr, "Checking reference integrity ... \n");
  BuildAlphabet();
  run = (uint8_t *) Calloc(Par.nTFiles, sizeof(uint8_t));
  ac_decoder_init(&acd, Par.tFileName[0]);
  ac_model_init(&acm, 2);
  if((markC = readNBits(64, &acd, &acm)) != MARK)
    {
    fprintf(stderr, "Error: unmatching compressed file!\n"
    "%s is not a compressed file by smash!\n", Par.tFileName[0]);
    exit(1);
    }
  CmpCheckSum(readNBits(64, &acd, &acm));
  Par.rc = readNBits(32, &acd, &acm);
  Par.rm = readNBits(32, &acd, &acm);
  Par.ri = readNBits( 1, &acd, &acm);
  Par.ra = readNBits(32, &acd, &acm);
  ac_model_done(&acm);
  ac_decoder_done(&acd);
  for(n = 1 ; n < Par.nTFiles ; ++n)
    {
    ac_decoder_init(&acd, Par.tFileName[n]);
    if((markC  = readNBits(64, &acd, &acm)) != MARK)
      {
      fprintf(stderr, "Error: unmatching compressed file!\n%s is not a "
      "compressed file by smash!\n", Par.tFileName[n]);
      exit(1);
      }
    ac_model_init(&acm, 2);
    CmpCheckSum(readNBits(64, &acd, &acm));
    rc = readNBits(32, &acd, &acm);
    rm = readNBits(32, &acd, &acm);
    ri = readNBits( 1, &acd, &acm);
    ra = readNBits(32, &acd, &acm);
    ac_model_done(&acm);
    ac_decoder_done(&acd);
    if(rc != Par.rc) ok = 1;
    if(rm != Par.rm) ok = 1;
    if(ri != Par.ri) ok = 1;
    if(ra != Par.ra) ok = 1;
    run[n] = (ok == 0 ? 0 : 1);
    if(run[n] == 1)
      CmpInputError(n);
    ok = 0;
    }
  return run;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
