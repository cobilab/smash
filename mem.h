#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "defs.h"
#include <stdio.h>
#include <stdint.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void     *Malloc     (size_t);
void     *Calloc     (size_t, size_t);
void     *Realloc    (void *, size_t, size_t);
void     Free        (void *);
U64      TotalMemory ();
U64      MaxMemory   ();

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif

