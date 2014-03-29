#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

typedef unsigned long long ULL;

typedef struct
  {
  uint8_t  help;
  uint8_t  verbose;
  uint8_t  force;
  uint32_t context;
  uint32_t alpha;
  uint8_t  ir;
  uint32_t hash;
  uint32_t seed;
  double   threshold;
  double   width;
  int64_t  window;
  int64_t  drop;
  int64_t  subsample;
  uint8_t  wType;
  uint64_t minimum;
  char     *output;
  uint64_t refSize;
  uint64_t tarSize;
  }
Parameters;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// SYSTEM VALUES:
#define BUFFER_SIZE            65535      
#define BUFFER_REV_SIZE        1048576
#define PROGRESS_MIN           1000
#define HASH_TABLE_BEGIN_CTX   15
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
#define DEFAULT_DROP           50000
#define DEFAULT_MINIMUM        1000000
#define DEFAULT_CX             50.0
#define DEFAULT_CY             90.0
#define DEFAULT_TX             50.0
#define DEFAULT_TY             82.0
#define DEFAULT_WIDTH          23.0
#define DEFAULT_RIGHTSHIFT     29.0

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

