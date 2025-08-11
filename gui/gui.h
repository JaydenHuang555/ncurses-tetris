#ifndef GUI_H
#define GUI_H

#include "bitdefs.h"
#include "rectangle.h"
#include "pthread.h"
#include <stdatomic.h>

struct gui_tile_info_t {
    size_t len;
    size_t offset;
    struct tile_t **tiles;
};

struct gui_t {
    struct rectangle_t *drop_frame; 
    struct rectangle_t *next_block_frame;
    struct gui_tile_info_t tile_info;
    pthread_mutex_t mutex;
    atomic_short running;
};

extern void gui_init(struct gui_t*);
extern u0 gui_deinit(struct gui_t*);

#endif
