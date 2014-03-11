#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

typedef struct
  {
  uint8_t  help;
  uint8_t  verbose;
  uint8_t  force;
  uint32_t context;
  uint32_t alpha;
  uint32_t hash;
  double   threshold;
  uint32_t window;
  uint32_t drop;
  uint64_t minimum;
  uint64_t refSize;
  uint64_t tarSize;
  }
Parameters;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// SYSTEM VALUES:
#define BUFFER_SIZE            1000000                                    //1M
#define PROGRESS_MIN           1000
#define HASH_TABLE_BEGIN_CTX   14
#define MAX_STRING_SIZE        1024
#define LEFT_BUFFER_GUARD      32
#define PRECISION              2

// DEFAULT VALUES:
#define DEFAULT_HELP           0
#define DEFAULT_FORCE          0
#define DEFAULT_VERBOSE        0
#define DEFAULT_CONTEXT        25
#define DEFAULT_ALPHA          1000
#define DEFAULT_MAX_COUNT      65535
#define DEFAULT_HASH_SIZE      29999999
#define DEFAULT_THRESHOLD      1.5
#define DEFAULT_WINDOW         100001
#define DEFAULT_DROP           50000
#define DEFAULT_MINIMUM        1000000

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

