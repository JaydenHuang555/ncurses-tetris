#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "rectangle.h"
#include "pthread.h"

extern void graphics_start(pthread_mutex_t*);
extern void graphics_end(void);
extern void graphics_register_rectangle(struct rectangle_t*);

#endif
