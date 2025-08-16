#ifndef TILE_H
#define TILE_H

#include "bitdefs.h"
#include "cords.h"
#include "colorpairs.h"
#include "graphics.h"

#define TILE_FACE_HEIGHT 2
#define TILE_FACE_WIDTH (TILE_FACE_HEIGHT * 2)

struct tile_t {
    struct rectangle_t *rec; 
    struct cords2d_t cords;
    s8 visible;
};

extern void tile_init(struct tile_t *tile, struct cords2d_t cords, enum colorpair_t);
extern void tile_publish(struct tile_t *tile, struct graphics_t *);

#endif
