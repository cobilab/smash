#include "mem.h"
#include <stdlib.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static U64 totalMemory = 0;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void *Malloc(size_t size)
  {
  void *pointer = malloc(size);

  if(pointer == NULL)
    {
    fprintf(stderr, "Error allocating %zu bytes.\n", size);
    exit(1);
    }

  totalMemory += size;

  return pointer;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void *Calloc(size_t nmemb, size_t size)
  {
  void *pointer = calloc(nmemb, size);

  if(pointer == NULL)
    {
    fprintf(stderr, "Error allocating %zu bytes.\n", size);
    exit(1);
    }

  totalMemory += nmemb * size;

  return pointer;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void *Realloc(void *ptr, size_t size, size_t additionalSize)
  {
  void *pointer = realloc(ptr, size);
	
  if(pointer == NULL) 
    {
    fprintf(stderr, "Error allocating %zu bytes.\n", size);
    exit(1);
    }

  totalMemory += additionalSize;

  return pointer;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Free(void *ptr)
  {
  free(ptr);
//  ptr = NULL;                                           // prevent double free
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

U64 TotalMemory()
  {
  return totalMemory;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
