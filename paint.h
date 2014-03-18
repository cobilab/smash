#ifndef PAINT_H_INCLUDED
#define PAINT_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void      PrintFinal   (FILE *);
void      PrintHead    (FILE *, double, double);
void      RecOval      (FILE *, double, double, double, double, char *);
void      RecOvalIR    (FILE *, double, double, double, double, char *);
void      Rect         (FILE *, double, double, double, double, char *);
void      RectIR       (FILE *, double, double, double, double, char *);
void      Chromosome   (FILE *, double, double, double, double);
void      Text         (FILE *, double, double, char *);
void      TextFloat    (FILE *, double, double, double);
double    GetPoint     (ULL);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
