#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "rectangle.h"
#include "pthread.h"
#include "bitdefs.h"

struct graphics_t {
   struct rectangle_t **rec;
   size_t rec_size, rec_cap;
   pthread_t redrawing_thread;
   pthread_mutex_t thread_lock, *ncurses_mutex;
   volatile s8 redrawing_thread_running;
   WINDOW *drawing_window;
};

extern void graphics_start(struct graphics_t *, pthread_mutex_t*);
extern void graphics_end(struct graphics_t*);
extern void graphics_register_rectangle(struct graphics_t*, struct rectangle_t*);

#endif
