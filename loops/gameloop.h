
#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "pthread.h"
#include "stdatomic.h"

extern pthread_mutex_t gameloop_g_thread_lock;

typedef int (*gameloop_runner_t)(void*);

struct gameloop_runner_exec_t {
	gameloop_runner_t runner;
	void *args;
	struct gameloop_runner_exec_t *next;
};

struct gameloop_t {
    pthread_t g_loop_thread;
    pthread_mutex_t gameloop_g_thread_lock;
    atomic_short g_is_loop_thread_running;
    struct gameloop_runner_exec_t *head, *tail;
};




extern void gameloop_start(struct gameloop_t*);
extern void gameloop_end(struct gameloop_t*);
extern void gameloop_add_runner(struct gameloop_t*, gameloop_runner_t, void*);


#endif
