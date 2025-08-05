
#include "stdlib.h"
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "stdio.h"
#include "threadutils.h"
#include "gameloop.h"
// #include "string.h"
#include "bitdefs.h"
#include "stdatomic.h"

#define FRAME_DELAY_USEC 16667

static void *callback(void *arg) {
	struct gameloop_t *gameloop = (struct gameloop_t*)arg;
	while(atomic_load(&gameloop->g_is_loop_thread_running)) {
		SYNC(gameloop_g_thread_lock, {
			struct gameloop_runner_exec_t *next;
			next = gameloop->head;
			while(next) {
				next->runner(next->args);
				next = next->next;
			}
		});
		usleep(FRAME_DELAY_USEC);
	}
	return arg;
}

static int gameloop_runner_dummy(u0 *raw) {
	return 0;
}

void gameloop_start(struct gameloop_t *gameloop) {
	gameloop->head = 0;
	gameloop->tail = 0;
	atomic_store(&gameloop->g_is_loop_thread_running, 0);
	if(pthread_create(&gameloop->g_loop_thread, 0, callback, gameloop)) {
		perror("unable to create gameloop thread\n");
		raise(SIGINT);
	}
	gameloop_add_runner(gameloop, gameloop_runner_dummy, 0);	
}

void gameloop_add_runner(struct gameloop_t *gameloop, gameloop_runner_t runner, void *args) {
	SYNC(gameloop_g_thread_lock, {
		struct gameloop_runner_exec_t *next = malloc(sizeof(struct gameloop_runner_exec_t));		
		if(!next) {
			perror("unable to malloc next gameloop runner exec\n");
			raise(SIGINT);
		}
		next->next = 0;
		next->runner = runner;
		next->args = args;
		if(!gameloop->head) {
			gameloop->head = next;
			gameloop->tail = gameloop->head;
		}
		else {
			gameloop->tail->next = next;
			gameloop->tail = gameloop->tail->next;
		}
	});
}

void gameloop_end(struct gameloop_t *gameloop) {
	atomic_store(&gameloop->g_is_loop_thread_running, 0);
	if(pthread_join(gameloop->g_loop_thread, 0)) {
		raise(SIGINT);
	}
	pthread_mutex_destroy(&gameloop_g_thread_lock);
	struct gameloop_runner_exec_t *next = gameloop->head;
	while(next) {
		struct gameloop_runner_exec_t *to_free = next;
		next = next->next;
		free(to_free);
	}
}
