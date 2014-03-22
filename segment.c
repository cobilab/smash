#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "segment.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char *SegmentSequence(char *fName, Parameters *P)
  {
  FILE     *Reader = NULL , *Writter = NULL;
  double   val, threshold;
  ULL      pos, initPosition, lastPosition;
  int      region;
  clock_t  stop, start;
  char     *fNameOut;

  if(P->verbose == 1)
    {
    start = clock();
    fprintf(stderr, "Segmenting ...\n");
    }

  threshold    = P->threshold; 
  Reader       = Fopen(fName, "r");
  fNameOut     = concatenate(fName, ".seg");
  Writter      = Fopen(fNameOut, "w");

  initPosition = 0;
  lastPosition = 0;
  if(fscanf(Reader, "%llu\t%lf", &pos, &val) == 2)
    region = val < threshold ? 0 : 1;
  else
    {
    fprintf(stderr, "Error: unknown format\n");
    exit(1);
    }
  while(fscanf(Reader, "%llu\t%lf", &pos, &val) == 2)
    {
    if(val > threshold && region == 0)
      {
      if(lastPosition != initPosition)
        {
        fprintf(Writter, "%llu:%llu\n", initPosition, lastPosition);
        region = 1;
        }
      }
    if(region == 1 && val < threshold)
      {
      region = 0;
      initPosition = pos;
      }
    lastPosition = pos;
    }

  if(region == 0 && (lastPosition > (initPosition + 10)))
    fprintf(Writter, "%llu:%llu\n", initPosition, lastPosition);

  fclose(Reader);
  fclose(Writter);
  unlink(fName);

  if(P->verbose == 1)
    {
    stop = clock();
    fprintf(stderr, "Done! Needed %g s for segmentation.\n", ((double) (stop -
    start)) / CLOCKS_PER_SEC);
    }

  return fNameOut;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

