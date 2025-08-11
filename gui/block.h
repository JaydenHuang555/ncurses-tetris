#ifndef BLOCK_H
#define BLOCK_H

#include "cords.h"
#include "tile.h"

enum block_shape_t {
    BLOCK_SHAPE_SQUARE
};

struct block_t {
    enum block_shape_t shape;
    struct cords2d_t center;
    struct tile_t tile;
};

extern void block_set_rec(struct block_t *);

#endif
