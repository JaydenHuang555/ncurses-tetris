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

static void *update_callback(void *arg) {
	struct graphics_t *graphics = (struct graphics_t*)arg;
	while(graphics->g_redrawing_thread_running) {
		SYNC(*graphics->g_ncurses_mutex, wclear(graphics->g_drawing_window););
		SYNC(graphics->g_thread_lock, {
			for(size_t i = 0; i < graphics->g_rec_size; i++) 
				if(graphics->g_rec[i]->filled) 
					SYNC(*graphics->g_ncurses_mutex, rectangle_draw_filled(graphics->g_rec[i], graphics->g_drawing_window));
				else SYNC(*graphics->g_ncurses_mutex, rectangle_draw_border(graphics->g_rec[i], graphics->g_drawing_window));
		});
		SYNC(*graphics->g_ncurses_mutex, {
			wrefresh(graphics->g_drawing_window);
		});
		usleep(FPS_60);
	}
	return arg;
}

void graphics_start(struct graphics_t *graphics, pthread_mutex_t *ncurses_mutex) {
	COLORPAIRS_INIT();
	graphics->g_ncurses_mutex = ncurses_mutex;
	graphics->g_rec_cap = 10;
	graphics->g_rec = (struct rectangle_t**)malloc(sizeof(struct rectangle_t*) * graphics->g_rec_cap);
	if(!graphics->g_rec) {
		perror("unable to malloc rectangle buffer\n");
		raise(SIGINT);
	}
	graphics->g_drawing_window = newwin(LINES, COLS, 0, 0);
	if(!graphics->g_drawing_window) {
		perror("unable to create drawing window");
		raise(SIGINT);
	}
	graphics->g_redrawing_thread_running = 1;
	if(pthread_create(&graphics->g_redrawing_thread, 0, update_callback, graphics)) {
		graphics->g_redrawing_thread_running = 0; 
		perror("unable to create the redrawing thread\n");
		raise(SIGINT);
	}
}

void graphics_end(struct graphics_t *graphics) {
	SYNC(graphics->g_thread_lock, {graphics->g_redrawing_thread_running = 0;});
	
	pthread_join(graphics->g_redrawing_thread, 0);
	pthread_mutex_destroy(&graphics->g_thread_lock);	

	if(graphics->g_rec) {
		for(size_t i = 0; i < graphics->g_rec_size; i++)
			if(graphics->g_rec[i]) rectangle_free(&graphics->g_rec[i]);
		free(graphics->g_rec);
		graphics->g_rec = 0;
	}
	
	delwin(graphics->g_drawing_window);
}

static u8 contains_rec(struct graphics_t *graphics, struct rectangle_t *rec) {
	size_t s;
	u8 err = 0;
	SYNC(graphics->g_thread_lock, {s = graphics->g_rec_size;});
	for(size_t i = 0; i < s; i++) {
		SYNC(graphics->g_thread_lock, {
			if(graphics->g_rec[i] == rec) {
				err = 1;
				break;
			}
		});
	}
	return err;
}

void graphics_register_rectangle(struct graphics_t *graphics, struct rectangle_t *rec) {
	if(contains_rec(graphics, rec)) {
		perror("can't add more than one of the same rectangle obj\n");
		raise(SIGINT);
	}
	SYNC(graphics->g_thread_lock, {
		graphics->g_rec[graphics->g_rec_size++] = rec;
	});
	u8 want_resize = graphics->g_rec_size == graphics->g_rec_cap;
	if(want_resize) {
		graphics->g_rec_cap *= 2;
		struct rectangle_t **next = (struct rectangle_t**)malloc(sizeof(struct rectangle_t*) * graphics->g_rec_cap);
		if(!next) {
			perror("unable to malloc next buffer for regrowing\n");
			raise(SIGINT);
		}
		SYNC(graphics->g_thread_lock, {
			for(size_t i = 0; i < graphics->g_rec_size; i++) next[i] = graphics->g_rec[i];
			free(graphics->g_rec);
			graphics->g_rec = next;
		});
	}
}
