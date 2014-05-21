#ifndef PAINT_H_INCLUDED
#define PAINT_H_INCLUDED

#include "defs.h"

typedef struct
  {
  U8  *backColor;
  DB  width;
  DB  cx; 
  DB  cy;
  DB  tx;
  DB  ty;
  DB  refSize;
  DB  tarSize;
  DB  maxSize;  
  }
Painter;

typedef struct
  {
  U8  r;
  U8  g;
  U8  b;
  } 
RgbColor;

typedef struct
  {
  U8  h;
  U8  s;
  U8  v;
  } 
HsvColor;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Painter   *CreatePainter (DB, DB, U8 *);
RgbColor  HsvToRgb       (HsvColor);
HsvColor  RgbToHsv       (RgbColor);
U8        *GetRgbColor   (U8);
void      PrintFinal     (FILE *);
void      PrintHead      (FILE *, DB, DB);
void      RectOval       (FILE *, DB, DB, DB, DB, U8 *);
void      RectOvalIR     (FILE *, DB, DB, DB, DB, U8 *);
void      Rect           (FILE *, DB, DB, DB, DB, U8 *);
void      RectIR         (FILE *, DB, DB, DB, DB, U8 *);
void      Chromosome     (FILE *, DB, DB, DB, DB);
void      Text           (FILE *, DB, DB, U8 *);
void      TextFloat      (FILE *, DB, DB, DB);
DB        GetPoint       (U64);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#endif
