#ifndef PAINT_H_INCLUDED
#define PAINT_H_INCLUDED

#include "defs.h"

typedef struct
  {
  char    *backColor;
  double  width;
  double  rightShift;
  double  cx; 
  double  cy;
  double  tx;
  double  ty;
  double  refSize;
  double  tarSize;
  double  maxSize;  
  }
Painter;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Painter   *CreatePainter (double, double, char *);
void      PrintFinal     (FILE *);
void      PrintHead      (FILE *, double, double);
void      RectOval       (FILE *, double, double, double, double, char *);
void      RectOvalIR     (FILE *, double, double, double, double, char *);
void      Rect           (FILE *, double, double, double, double, char *);
void      RectIR         (FILE *, double, double, double, double, char *);
void      Chromosome     (FILE *, double, double, double, double);
void      Text           (FILE *, double, double, char *);
void      TextFloat      (FILE *, double, double, double);
double    GetPoint       (ULL);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
