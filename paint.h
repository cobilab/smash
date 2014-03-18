#ifndef PAINT_H_INCLUDED
#define PAINT_H_INCLUDED

#include "defs.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void      RecOval      (double, double, double, double, char *);
void      RecOvalIR    (double, double, double, double, char *);
void      Rect         (double, double, double, double, char *);
void      RectIR       (double, double, double, double, char *);
void      Chromosome   (double, double, double, double);
void      Text         (double, double, char *);
void      TextFloat    (double, double, double);
double    GetPoint     (ULL);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
