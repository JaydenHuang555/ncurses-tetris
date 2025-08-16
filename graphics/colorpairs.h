#ifndef COLORPAIRS_H
#define COLORPAIRS_H

#include "ncurses.h"

enum colorpair_t {
   COLORPAIR_DEFAULT = 0,
   COLORPAIR_WHITE = 1,
   COLORPAIR_RED = 2,
   COLORPAIR_BLUE = 3,
   COLORPAIR_GREEN = 4,
};

#define COLORPAIRS_INIT() {\
   init_pair(COLORPAIR_WHITE, COLOR_WHITE, COLOR_WHITE);\
   init_pair(COLORPAIR_RED, COLOR_RED, COLOR_RED);\
   init_pair(COLORPAIR_BLUE, COLOR_BLUE, COLOR_BLUE);\
}\

#define COLORPAIR_ATTR(win, colorpair, block) \
do { \
   wattron(win, COLOR_PAIR(colorpair)); \
   wattron(win, A_BOLD);\
   block; \
   wattroff(win, A_BOLD);\
   wattroff(win, COLOR_PAIR(colorpair));\
} while (0)

#endif
