#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "ncurses.h"
#include "colorpairs.h"
#include "cords.h"
#include "bitdefs.h"

struct rectangle_t {
    struct cords2d_t cords_top_left;
    struct cords2d_t cords_bottom_right;
    s8 filled;
    s8 visibile;
    enum colorpair_t color;
};

extern struct rectangle_t *rectangle_malloc_with_cords(s32 y1, s32 x1, s32 y2, s32 x2);
extern void rectangle_draw_border(struct rectangle_t*, WINDOW*);
extern void rectangle_free(struct rectangle_t**);
extern void rectangle_draw_filled(struct rectangle_t*, WINDOW*);
#endif
