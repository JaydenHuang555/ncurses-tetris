#ifndef BLOCK_H
#define BLOCK_H

#include "rectangle.h"
#include "cords.h"

enum block_shape_t {
    BLOCK_SHAPE_SQUARE
};

struct block_t {
    enum block_shape_t shape;
    struct cords2d_t center;
    struct rectangle_t *rec;
};

extern void block_set_rec(struct block_t *);

#endif
