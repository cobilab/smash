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
  double   threshold;
  uint32_t window;
  uint32_t drop;
  uint64_t minimum;
  }
Parameters;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define BUFFER_SIZE            1000000                                    //1M
#define PROGRESS_MIN           1000
#define HASH_TABLE_BEGIN_CTX   14

#define DEFAULT_HELP           0
#define DEFAULT_FORCE          0
#define DEFAULT_VERBOSE        0
#define DEFAULT_CONTEXT        25
#define DEFAULT_ALPHA          1000
#define DEFAULT_THRESHOLD      1.5
#define DEFAULT_WINDOW         100001
#define DEFAULT_DROP           50000
#define DEFAULT_MINIMUM        1000000
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

