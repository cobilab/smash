#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

#define DRAND48  1

typedef uint64_t U64;
typedef uint32_t U32;
typedef uint16_t U16;
typedef uint8_t  U8;
typedef int64_t  I64;
typedef int32_t  I32;
typedef int16_t  I16;
typedef int8_t   I8;
typedef double   DB;
typedef float    FL;

typedef struct
  {
  U8   help;
  U8   verbose;
  U8   force;
  U32  context;
  U32  alpha;
  U8   ir;
  U32  hash;
  U32  seed;
  DB   threshold;
  DB   width;
  I64  window;
  I64  subsample;
  U8   wType;
  U64  minimum;
  U8   *output;
  U64  refSize;
  U64  tarSize;
  }
Parameters;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// SYSTEM VALUES:
#define N_SYMBOLS              4
#define BUFFER_SIZE            65535      
#define BUFFER_REV_SIZE        1048576
#define PROGRESS_MIN           1000
#define MAX_STRING_SIZE        1024
#define LEFT_BUFFER_GUARD      32
#define PRECISION              2
#define SEED_JUMP              11
#define W_TYPE                 0                        // DEFAULT WINDOW TYPE
#define W_HAMMING              0
#define W_HANN                 1
#define W_BLACKMAN             2
#define W_RECTANGULAR          3
#define REF                    0
#define TAR                    1
#define WINDOW_RATIO           1300
#define SUBSAMPLE_RATIO        5
#define LEVEL_SATURATION       160
#define LEVEL_VALUE            160
#define LOW_REGION             0
#define HIGH_REGION            1

// DEFAULT VALUES:
#define DEFAULT_HELP           0
#define DEFAULT_FORCE          0
#define DEFAULT_VERBOSE        0
#define DEFAULT_CONTEXT        20
#define DEFAULT_ALPHA          1000
#define DEFAULT_IR             0
#define DEFAULT_MAX_COUNT      65535
#define DEFAULT_HASH_SIZE      29999999
#define DEFAULT_SEED           -1
#define DEFAULT_THRESHOLD      1.5
#define DEFAULT_WINDOW         -1
#define DEFAULT_WIN_TYPE       0
#define DEFAULT_SUBSAMPLE      50000
#define DEFAULT_SAMPLE_RATIO   10000
#define DEFAULT_MINIMUM        1000000
#define DEFAULT_CX             70.0
#define DEFAULT_CY             75.0
#define DEFAULT_TX             50.0
#define DEFAULT_TY             82.0
#define DEFAULT_WIDTH          30.0
#define DEFAULT_SPACE          10.0
#define EXTRA                  150 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

