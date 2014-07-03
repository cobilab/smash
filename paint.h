#ifndef PAINT_H_INCLUDED
#define PAINT_H_INCLUDED

#include "defs.h"

typedef struct
  {
  uint8_t *backColor;
  double  width;
  double  cx; 
  double  cy;
  double  tx;
  double  ty;
  double  refSize;
  double  tarSize;
  double  maxSize;  
  }
Painter;

typedef struct
  {
  uint8_t  r;
  uint8_t  g;
  uint8_t  b;
  } 
RgbColor;

typedef struct
  {
  uint8_t  h;
  uint8_t  s;
  uint8_t  v;
  } 
HsvColor;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Painter   *CreatePainter (double, double, uint8_t *);
RgbColor  HsvToRgb       (HsvColor);
HsvColor  RgbToHsv       (RgbColor);
uint8_t   *GetRgbColor   (uint8_t);
void      PrintFinal     (FILE *);
void      PrintHead      (FILE *, double, double);
void      RectOval       (FILE *, double, double, double, double, uint8_t *);
void      RectOvalIR     (FILE *, double, double, double, double, uint8_t *);
void      Rect           (FILE *, double, double, double, double, uint8_t *);
void      RectIR         (FILE *, double, double, double, double, uint8_t *);
void      Chromosome     (FILE *, double, double, double, double);
void      Text           (FILE *, double, double, uint8_t *);
void      Textfloatoat   (FILE *, double, double, double);
double    GetPoint       (uint64_t);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
