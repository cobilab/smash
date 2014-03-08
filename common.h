#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "context.h"
#include "ac.h"
#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FILE        *Fopen           (const char *, const char *);
void        FillLogTable     (uint32_t    , uint32_t    , uint32_t);
double      SearchLog        (uint32_t );
uint32_t    FLog2            (uint64_t );
uint8_t     GetComp          (uint8_t  );
uint8_t     GetComp2         (uint8_t  );
uint64_t    NBytesInFile     (const char *);
uint8_t     *ReverseStr      (uint8_t *, uint32_t);
char        *CloneString     (char *   );
char        *RepString       (const char *, const char *, const char *);
uint32_t    ArgsNumber       (uint32_t , char *[], uint32_t, char *);
uint8_t     ArgsState        (uint8_t  , char *[], uint32_t, char *);
char        *ArgsString      (char    *, char *[], uint32_t, char *);
uint32_t    ReadFNames       (char    *, char *[], uint32_t);
void        CreaDecFNames    (void     );
void        FAccessWPerm     (char    *);
void        PrintArgs        (void     );
uint64_t    CheckAlphabet    (char    *);
void        BuildAlphabet    (void     );
void        LoadReference    (void     );
void        CmpCheckSum      (uint64_t );
void        CmpInputError    (uint32_t );
inline void CalcProgress     (uint64_t , uint64_t);
uint8_t     *CheckReference  (void     );

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
