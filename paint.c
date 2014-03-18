#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "paint.h"
#include "common.h"
#include "mem.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RectOval(double w, double h, double x, double y, char *color)
  {
  printf("<rect "
              "style=\"fill:%s;fill-opacity:1;stroke-width:2;"
              "stroke-miterlimit:4;stroke-dasharray:none\" "
              "id=\"rectx\" "
              "width=\"%.2lf\" " 
              "height=\"%.2lf\" "
              "x=\"%.2lf\" "
              "y=\"%.2lf\" "
              "ry=\"12.5\" "
           "/>\n", color, w, h, x, y);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RectOvalIR(double w, double h, double x, double y, char *color)
  {
  RectOval(w, h, x, y, color);
  printf("<rect "
              "style=\"fill-opacity:1;stroke-width:2;"
              "stroke-miterlimit:4;stroke-dasharray:none"
              "stroke-dasharray:none;fill:url(#xtrace);"
              "fill-rule:nonzero;opacity:1\" "
              "id=\"recty\" "
              "width=\"%.2lf\" " 
              "height=\"%.2lf\" "
              "x=\"%.2lf\" "
              "y=\"%.2lf\" "
              "ry=\"12.5\" "
           "/>\n", w, h, x, y);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Rect(double w, double h, double x, double y, char *color)
  {
  printf("<rect style=\"fill:%s;fill-opacity:1;stroke-width:2;"
              "stroke-miterlimit:4;stroke-dasharray:none\" "
              "id=\"rect3777\" "
              "width=\"%.2lf\" " 
              "height=\"%.2lf\" "
              "x=\"%.2lf\" "
              "y=\"%.2lf\" "
              "ry=\"0\" "
           "/>\n", color, w, h, x, y);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RectIR(double w, double h, double x, double y, char *color)
  {
  Rect(w, h, x, y, color);
  printf("<rect "
              "style=\"fill-opacity:1;stroke-width:2;stroke-miterlimit:4;"
              "stroke-dasharray:none;fill:url(#Wavy);fill-rule:"
              "nonzero;opacity:1\" "
              "id=\"rect6217\" "
              "width=\"%.2lf\" " 
              "height=\"%.2lf\" "
              "x=\"%.2lf\" "
              "y=\"%.2lf\" "
              "ry=\"0\" "
           "/>\n", w, h, x, y);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Chromosome(double w, double h, double x, double y)
  {
  char borderColor[] = "#000000";
  double wk = w / 2 + 0.5;

  printf("<path "
         "d=\"m %.2lf,"
         "%.2lf 0,"
         "%.2lf c 0, -8.31 6.69, -%.2lf %.2lf, -%.2lf l -%.2lf,0 z m %.2lf,"
         "0 c 8.31,0 %.2lf,6.69 %.2lf,%.2lf l 0,-%.2lf -%.2lf,0 z\" "
         "id=\"rect3787\" style=\"fill:#fff;fill-opacity:1;fill-rule:"
         "nonzero;stroke:none\" />", x-0.5, y-0.5, 
         wk, wk, wk, wk, wk, wk, wk, wk, wk, wk, wk);

  printf("<path "
         "d=\"m %.2lf,"
         "%.2lf 0,"
         "-%.2lf c 0,8.31 -6.69, %.2lf -%.2lf, %.2lf l %.2lf,0 z m -%.2lf,"
         "0 c -8.31,0 -%.2lf,-6.69 -%.2lf,-%.2lf l 0,%.2lf %.2lf,0 z\" "
         "id=\"rect3787\" style=\"fill:#fff;fill-opacity:1;fill-rule:"
         "nonzero;stroke:none\" />", x+0.5+w, y+0.5+h, 
         wk, wk, wk, wk, wk, wk, wk, wk, wk, wk, wk);

  printf("<rect style=\"fill:none;stroke:%s;stroke-width:2;"
            "stroke-linecap:butt;stroke-linejoin:miter;"
            "stroke-miterlimit:4;stroke-opacity:1;"
            "stroke-dasharray:none\" "
            "id=\"rect2985\" "
            "width=\"%.2lf\" " 
            "height=\"%.2lf\" "
            "x=\"%.2lf\" "
            "y=\"%.2lf\" "
            "ry=\"12.5\" "
          "/>\n", borderColor, w, h, x, y);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Text(double x, double y, char *name)
  {
  printf("<text xml:space=\"preserve\" "
            "style=\"font-size:40px;font-style:normal;"
            "font-weight:normal;line-height:125%%;"
            "letter-spacing:0px;word-spacing:0px;fill:#000000;"
            "fill-opacity:1;stroke:none;font-family:Sans\" "
            "x=\"%.2lf\" "
            "y=\"%.2lf\" "
            "id=\"corben\" "
            "sodipodi:linespacing=\"125%%\">"
            "<tspan sodipodi:role=\"line\" "
            "id=\"tspan3804\" "
            "x=\"%.2lf\" "
            "y=\"%.2lf\" "
            "style=\"font-size:18px;font-style:normal;font-variant:"
            "normal;font-weight:normal;font-stretch:normal;"
            "text-align:start;line-height:125%%;writing-mode"
            ":lr-tb;text-anchor:start;font-family:Arial;-"
            "inkscape-font-specification:Arial\">%s</tspan>\n"
         "</text>\n", x, y, x, y, name);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TextFloat(double x, double y, double name)
  {
  printf("<text xml:space=\"preserve\" "
             "style=\"font-size:40px;font-style:normal;"
             "font-weight:normal;line-height:125%%;"
             "letter-spacing:0px;word-spacing:0px;fill:#000000;"
             "fill-opacity:1;stroke:none;font-family:Sans\" "
             "x=\"%.2lf\" "
             "y=\"%.2lf\" "
             "id=\"corben\" "
             "sodipodi:linespacing=\"125%%\">"
             "<tspan sodipodi:role=\"line\" "
             "id=\"tspan3804\" "
             "x=\"%.2lf\" "
             "y=\"%.2lf\" "
             "style=\"font-size:18px;font-style:normal;font-variant:"
             "normal;font-weight:normal;font-stretch:normal;"
             "text-align:start;line-height:125%%;writing-mode"
             ":lr-tb;text-anchor:start;font-family:Arial;-"
             "inkscape-font-specification:Arial\"> %5.2lf</tspan>\n"
          "</text>\n", x, y, x, y, name);
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

double GetPoint(ULL p)
  {
  return p / 1000000.0 * 5;
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
