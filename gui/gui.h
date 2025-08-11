#ifndef GUI_H
#define GUI_H

#include "bitdefs.h"
#include "rectangle.h"

struct gui_t {
    struct rectangle_t *drop_frame; 
    struct rectangle_t *next_block_frame;
    struct rectangle_t *blocks;
};

extern void gui_init(struct gui_t*);
extern u0 gui_deinit(struct gui_t*);

#endif
