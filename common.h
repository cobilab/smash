#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "context.h"
#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef struct
  {
  U64  init;                                       // Initial pattern position 
  U64  end;                                            // End pattern position 
  }
PatEntry;

typedef struct
  {
  U32      nPatterns;                                    // Number of patterns
  PatEntry *p;
  }
Patterns;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FILE        *Fopen           (const char *, const char *);
void        FillLogTable     (uint32_t, uint32_t, uint32_t);
double      SearchLog        (uint32_t );
uint32_t    FLog2            (uint64_t );
uint8_t     DNASymToNum      (uint8_t  );
uint8_t     GetCompNum       (uint8_t  );
uint8_t     GetCompSym       (uint8_t  );
uint64_t    NBytesInFile     (FILE *);
uint64_t    FopenBytesInFile (const char *);
uint8_t     *ReverseStr      (uint8_t *, uint32_t);
char        *CloneString     (char *   );
char        *RepString       (const char *, const char *, const char *);
uint32_t    ArgsNumber       (uint32_t , char *[], uint32_t, char *);
double      ArgsDouble       (double, char *[], uint32_t, char *);
uint8_t     ArgsState        (uint8_t  , char *[], uint32_t, char *);
uint8_t     Args3State       (uint8_t  , char *[], uint32_t, char *);
char        *ArgsString      (char    *, char *[], uint32_t, char *);
char        *ArgsFiles       (char *[], uint32_t, char *);
void        FAccessWPerm     (char    *);
void        PrintArgs        (void     );
void        CmpInputError    (uint32_t );
inline void CalcProgress     (uint64_t , uint64_t);
char        *concatenate     (char *   , char *);
Patterns    *GetPatterns     (char *);
char        *ExtractSubSeq   (char *, Parameters *, uint64_t, uint64_t);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
