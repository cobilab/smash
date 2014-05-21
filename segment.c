#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "segment.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

U8 *SegmentSequence(U8 *fName, Parameters *P)
  {
  FILE     *Reader = NULL , *Writter = NULL;
  FL       val, threshold;
  U64      pos, initPosition, lastPosition;
  I32      region;
  clock_t  stop, start;
  U8       *fNameOut;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Segmenting ...\n");
    }

  threshold    = (FL) P->threshold; 
  Reader       = Fopen(fName, "r");
  fNameOut     = concatenate(fName, ".seg");
  Writter      = Fopen(fNameOut, "w");

  if(fscanf(Reader, "%llu\t%f", &pos, &val) == 2)
    region = val < threshold ? LOW_REGION : HIGH_REGION;
  else
    {
    fprintf(stderr, "Error: unknown format\n");
    exit(1);
    }
  initPosition = 1;
  lastPosition = pos;

  while(fscanf(Reader, "%llu\t%f", &pos, &val) == 2)
    {
    if(val >= threshold)
      { 
      if(region == LOW_REGION)
        {
        region = HIGH_REGION;
        fprintf(Writter, "%llu:%llu\n", initPosition, pos);
        }
      }

    else // val < threshold ====> LOW_REGION
      {
      if(region == HIGH_REGION)
        {
        region       = LOW_REGION;
        initPosition = pos;
        }
      }

    lastPosition = pos;
    }

  if(region == LOW_REGION)
    fprintf(Writter, "%llu:%llu\n", initPosition, lastPosition);

  fclose(Reader);
  fclose(Writter);
  unlink(fName);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Needed %g s for segmentation.\n", ((DB) (stop-
    start)) / CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

