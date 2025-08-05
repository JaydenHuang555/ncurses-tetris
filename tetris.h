#ifndef TETRIS_H
#define TETRIS_H

#include "pthread.h"
#include "graphics.h"
#include "gameloop.h"

#define VERSION 0.3

enum build_type_t {
    BUILD_TYPE_DEV = 0,
    BUILD_TYPE_STD = 1
};

#define BUILD_TYPE BUILD_TYPE_DEV

extern pthread_mutex_t ncurses_mutex;
extern struct graphics_t graphics;
extern struct gameloop_t gameloop;

#endif
