#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "ncurses.h"
#include "colorpairs.h"
// TODO: replace with cords instead
struct rectangle_t {
    int y1; // top left
    int x1; // top left
    int y2; // bottom right
    int x2; // bottom right

    char filled, visibile;
    enum colorpair_t color;
};

extern struct rectangle_t *rectangle_malloc_with_cords(int y1, int x1, int y2, int x2);
extern void rectangle_draw_border(struct rectangle_t*, WINDOW*);
extern void rectangle_free(struct rectangle_t**);
extern void rectangle_draw_filled(struct rectangle_t*, WINDOW*);
#endif
