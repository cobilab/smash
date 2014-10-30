#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "segment.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t *SegmentSequence(uint8_t *fName, Parameters *P)
  {
  FILE     *Reader = NULL , *Writter = NULL;
  float    val, threshold;
  uint64_t pos, initPosition, lastPosition;
  int32_t  region;
  clock_t  stop, start;
  uint8_t  *fNameOut;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Segmenting ...\n");
    }

  threshold    = (float) P->threshold; 
  Reader       = Fopen(fName, "r");
  fNameOut     = concatenate(fName, ".seg");
  Writter      = Fopen(fNameOut, "w");

  if(fscanf(Reader, "%"PRIu64"\t%f", &pos, &val) == 2)
    region = val < threshold ? LOW_REGION : HIGH_REGION;
  else
    {
    fprintf(stderr, "Error: unknown format\n");
    exit(1);
    }
  initPosition = 1;
  lastPosition = pos;

  while(fscanf(Reader, "%"PRIu64"\t%f", &pos, &val) == 2)
    {
    if(val >= threshold)
      { 
      if(region == LOW_REGION)
        {
        region = HIGH_REGION;
        fprintf(Writter, "%"PRIu64":%"PRIu64"\n", initPosition, pos);
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
    fprintf(Writter, "%"PRIu64":%"PRIu64"\n", initPosition, lastPosition);

  fclose(Reader);
  fclose(Writter);
  
  if(P->del) Unlink(fName);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Needed %g s for segmentation.\n", ((double) (stop-
    start)) / CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

