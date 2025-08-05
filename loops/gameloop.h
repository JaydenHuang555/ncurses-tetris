#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "pthread.h"

extern pthread_mutex_t gameloop_g_thread_lock;

typedef int (*gameloop_runner_t)(void*);

extern void gameloop_start(void);
extern void gameloop_end(void);
extern void gameloop_add_runner(gameloop_runner_t, void*);


#endif
