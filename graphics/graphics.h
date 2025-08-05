#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "rectangle.h"
#include "pthread.h"
#include "bitdefs.h"

struct graphics_t {
   struct rectangle_t **g_rec;
   size_t g_rec_size, g_rec_cap;
   pthread_t g_redrawing_thread;
   pthread_mutex_t g_thread_lock, *g_ncurses_mutex;
   volatile s8 g_redrawing_thread_running;
   WINDOW *g_drawing_window;
};

extern void graphics_start(struct graphics_t *, pthread_mutex_t*);
extern void graphics_end(struct graphics_t*);
extern void graphics_register_rectangle(struct graphics_t*, struct rectangle_t*);

#endif
