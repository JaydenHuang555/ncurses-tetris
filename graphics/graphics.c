#include <stddef.h>
#include <stdlib.h>
#include "bitdefs.h"
#include <pthread.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include "graphics.h"
#include "threadutils.h"
#include "colorpairs.h"

#define STARTED g_rec
#define FPS_60 16000 

static struct rectangle_t **g_rec = 0;
static size_t g_rec_size = 0, g_rec_cap = 0;
static pthread_t g_redrawing_thread;
static pthread_mutex_t g_thread_lock = PTHREAD_MUTEX_INITIALIZER, *g_ncurses_mutex;
static volatile s8 g_redrawing_thread_running = 0;
static WINDOW *g_drawing_window = 0;

static void *update_callback(void *arg) {
	while(g_redrawing_thread_running) {
		SYNC(*g_ncurses_mutex, wclear(g_drawing_window););
		// wclear(g_drawing_window);
		SYNC(g_thread_lock, {
			for(size_t i = 0; i < g_rec_size; i++) 
				if(g_rec[i]->filled) 
					SYNC(*g_ncurses_mutex, rectangle_draw_filled(g_rec[i], g_drawing_window));
				else SYNC(*g_ncurses_mutex, rectangle_draw_border(g_rec[i], g_drawing_window));
		});
		SYNC(*g_ncurses_mutex, {
			wrefresh(g_drawing_window);
			// usleep(FPS_60);
		});
		// wrefresh(g_drawing_window);
		usleep(FPS_60);
	}
	return arg;
}

void graphics_start(pthread_mutex_t *ncurses_mutex) {
	COLORPAIRS_INIT();
	g_ncurses_mutex = ncurses_mutex;
	g_rec_cap = 10;
	g_rec = (struct rectangle_t**)malloc(sizeof(struct rectangle_t*) * g_rec_cap);
	if(!g_rec) {
		perror("unable to malloc rectangle buffer\n");
		raise(SIGINT);
	}
	g_drawing_window = newwin(LINES, COLS, 0, 0);
	if(!g_drawing_window) {
		perror("unable to create drawing window");
		raise(SIGINT);
	}
	g_redrawing_thread_running = 1;
	if(pthread_create(&g_redrawing_thread, 0, update_callback, 0)) {
		g_redrawing_thread_running = !g_redrawing_thread_running;
		perror("unable to create the redrawing thread\n");
		raise(SIGINT);
	}
}

void graphics_end(void) {
	SYNC(g_thread_lock, {g_redrawing_thread_running = 0;});
	
	pthread_join(g_redrawing_thread, 0);
	pthread_mutex_destroy(&g_thread_lock);	

	if(g_rec) {
		for(size_t i = 0; i < g_rec_size; i++)
			if(g_rec[i]) rectangle_free(&g_rec[i]);
		free(g_rec);
		g_rec = 0;
	}
	
	delwin(g_drawing_window);
}

static u8 contains_rec(struct rectangle_t *rec) {
	size_t s;
	u8 err = 0;
	SYNC(g_thread_lock, {s = g_rec_size;});
	for(size_t i = 0; i < s; i++) {
		SYNC(g_thread_lock, {
			if(g_rec[i] == rec) {
				err = 1;
				break;
			}
		});
	}
	return err;
}

void graphics_register_rectangle(struct rectangle_t *rec) {
	if(contains_rec(rec)) {
		perror("can't add more than one of the same rectangle obj\n");
		raise(SIGINT);
	}
	SYNC(g_thread_lock, {
		g_rec[g_rec_size++] = rec;
	});
	u8 want_resize = g_rec_size == g_rec_cap;
	if(want_resize) {
		g_rec_cap *= 2;
		struct rectangle_t **next = (struct rectangle_t**)malloc(sizeof(struct rectangle_t*) * g_rec_cap);
		if(!next) {
			perror("unable to malloc next buffer for regrowing\n");
			raise(SIGINT);
		}
		SYNC(g_thread_lock, {
			for(size_t i = 0; i < g_rec_size; i++) next[i] = g_rec[i];
			free(g_rec);
			g_rec = next;
		});
	}
}
