#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef uint16_t ACCounter;              //Size of context counters for arrays
typedef uint8_t  HCCounter;         //Size of context counters for hash tables 
typedef uint64_t ULL;             
typedef uint8_t  UChar;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t       garbage;                             // WARNING: global variable

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct
  {
  uint8_t     verbose;
  uint8_t     help;
  uint8_t     force;
  }
Flags;

typedef struct
  {
  uint32_t    tc;
  uint32_t    tm;
  uint8_t     ti;
  uint32_t    rc;
  uint32_t    rm;
  uint8_t     ri;
  uint32_t    ra;
  uint32_t    b;
  char        *rFileName;
  char        **tFileName;
  char        **dFileName;
  uint32_t    nTFiles;
  Flags       F;
  }
Parameters;

typedef struct
  {
  uint8_t     *norAlp;
  uint8_t     *alp;
  uint8_t     *mask;
  uint32_t    nSym;
  uint64_t    rFileSize;
  uint64_t    rCheckSum;
  }
Alphabet;

typedef struct
  {
  uint32_t    nFiles;
  uint32_t    id;
  uint64_t    bits;
  uint64_t    size;
  }
Data;

typedef struct
  {
  uint64_t   key;                               //The key stored in this entry 
  HCCounter  *counters;                                 //The context counters 
  }
Entry;

typedef struct
  {
  uint16_t   *entrySize;                        //Number of keys in this entry
  Entry      **entries;                    //The heads of the hash table lists
  HCCounter  **zeroCounters;
  uint32_t   nUsedEntries;
  uint32_t   nUsedKeys;
  }
HashTable;


typedef struct
  {
  ACCounter  *counters;                                        // FCM Counters
  uint64_t   multiplier;                                 // Calculated product
  uint64_t   nPModels;                  //Maximum number of probability models
  uint32_t   ctxSize;                     // Current depth of context template
  uint32_t   nSymbols;                             // Number of coding symbols
  uint64_t   pModelIdx;                        // Index of probabilistic model
  uint64_t   idxIr;           // Inverted repeats index of probabilistic model
  uint32_t   deltaDen;       // Delta denominator (for probability estimation)
  uint32_t   maxC;                     // Maxmimum probabilistic model counter
  HashTable  hTable;
  }
CModel;

typedef struct
  {
  uint32_t   nActive;                              // Number of active threads
  uint8_t    *active;                         // Active or not for each thread
  uint8_t    *p;                         // Current percentage for each thread
  uint8_t    cur;                                        // Current percentage
  }
Percent;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

CModel       *cRef;
Parameters   Par;
Alphabet     Alpha;
Percent      Per;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define MARK                     125112013    //VERSION:12, RELEASE:25,11,2013
#define MAX_LOG_TABLE            160000000                    //MAX: 305.18 MB
#define MAX_FNAME_SIZE           1024
#define DEXT                     "d"
#define EXTINF                   "inf"
#define EXT                      "sm"
#define PROGRESS_MIN             1000
#define DEFAULT_BS               100
#define PRECISION                2


#define MAX_ALPHA_DEN            50
#define D_CTX_LOW_ORDER          4
#define HASH_SIZE                39999999    //19999999
#define MAX_OUT_LS               4096
#define MAX_SFIELD               512
#define L_GUARD_BUF              256
#define BUF_SIZE_CAT             1000000            // CAT DECOMPRESSED FILES
#define BUF_SIZE                 65536                     
#define CBUF_SIZE                65536                           //used in ac
#define BREAK_CHAR               10
#define MAX_FILESIZE             512
#define MAXSIZEA                 255
#define INVALID_S                255
#define NSIGNF                   256
#define MAX_ARG_SIZE             1024
         //ALERT: ASSERT THAT 2^Code_value_bits > nSymbols * MAXCNT * alphaDen
#define MAXCNT			 (((uint64_t) 1 << (sizeof(ACCounter)*8)) - 2)
#define MAXCNTR			 (((uint64_t) 1 << (sizeof(HCCounter)*8)) - 2)
#define MAX_ASIZE                100
#define SCHAR			 sizeof(uint8_t)
#define MAX_SIZE_BITS		 UINT32_MAX
#define NBITS_UINT32             sizeof(uint32_t) * 8
#define NBITS_UINT64             sizeof(uint64_t) * 8
#define MAX(a,b)                 ((a) > (b) ? a : b)
#define MIN(a,b)                 ((a) < (b) ? a : b)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

