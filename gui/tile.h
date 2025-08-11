#ifndef TILE_H
#define TILE_H

#include "bitdefs.h"
#include "cords.h"
#include "colorpairs.h"
#include "graphics.h"

#define TILE_FACE_LEN 3
#define TILE_AREA TILE_LEN * 2

struct tile_t {
    struct rectangle_t *rec; 
    struct cords2d_t cords;
    // enum colorpair_t color;
};

extern void tile_init(struct tile_t *tile, struct cords2d_t cords, enum colorpair_t);
extern void tile_publish(struct tile_t *tile, struct graphics_t *);

#endif
