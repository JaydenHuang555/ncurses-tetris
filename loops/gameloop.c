
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

static pthread_t g_loop_thread;
pthread_mutex_t gameloop_g_thread_lock = PTHREAD_MUTEX_INITIALIZER;
static atomic_short g_is_loop_thread_running = 0;

struct gameloop_runner_exec_t {
	gameloop_runner_t runner;
	void *args;
	struct gameloop_runner_exec_t *next;
};

static struct gameloop_runner_exec_t *head = 0, *tail = 0;

void *gameloop(void *arg) {
	while(atomic_load(&g_is_loop_thread_running)) {
		SYNC(gameloop_g_thread_lock, {
			struct gameloop_runner_exec_t *next;
			next = head;
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

void gameloop_start(void) {
	atomic_store(&g_is_loop_thread_running, 0);
	if(pthread_create(&g_loop_thread, 0, gameloop, 0)) {
		perror("unable to create gameloop thread\n");
		raise(SIGINT);
	}
	gameloop_add_runner(gameloop_runner_dummy, 0);	
}

void gameloop_add_runner(gameloop_runner_t runner, void *args) {
	SYNC(gameloop_g_thread_lock, {
		struct gameloop_runner_exec_t *next = malloc(sizeof(struct gameloop_runner_exec_t));		
		if(!next) {
			perror("unable to malloc next gameloop runner exec\n");
			raise(SIGINT);
		}
		next->next = 0;
		next->runner = runner;
		next->args = args;
		if(!head) {
			head = next;
			tail = head;
		}
		else {
			tail->next = next;
			tail = tail->next;
		}
	});
}

void gameloop_end(void) {
	atomic_store(&g_is_loop_thread_running, 0);
	if(pthread_join(g_loop_thread, 0)) {
		raise(SIGINT);
	}
	pthread_mutex_destroy(&gameloop_g_thread_lock);
	struct gameloop_runner_exec_t *next = head;
	while(next) {
		struct gameloop_runner_exec_t *to_free = next;
		next = next->next;
		free(to_free);
	}
}
