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

static u0 *update_callback(void *arg) {
	struct graphics_t *graphics = (struct graphics_t*)arg;
	while(graphics->redrawing_thread_running) {
		SYNC(*graphics->ncurses_mutex, wclear(graphics->drawing_window););
		SYNC(graphics->thread_lock, {
			for(size_t i = 0; i < graphics->rec_size; i++) 
				if(graphics->rec[i]->filled) 
					SYNC(*graphics->ncurses_mutex, rectangle_draw_filled(graphics->rec[i], graphics->drawing_window));
				else SYNC(*graphics->ncurses_mutex, rectangle_draw_border(graphics->rec[i], graphics->drawing_window));
		});
		SYNC(*graphics->ncurses_mutex, {
			wrefresh(graphics->drawing_window);
		});
		usleep(FPS_60);
	}
	return arg;
}

u0 graphics_start(struct graphics_t *graphics, pthread_mutex_t *ncurses_mutex) {
	COLORPAIRS_INIT();
	graphics->ncurses_mutex = ncurses_mutex;
	graphics->rec_cap = 10;
	graphics->rec = (struct rectangle_t**)malloc(sizeof(struct rectangle_t*) * graphics->rec_cap);
	if(!graphics->rec) {
		perror("unable to malloc rectangle buffer\n");
		raise(SIGINT);
	}
	graphics->drawing_window = newwin(LINES, COLS, 0, 0);
	if(!graphics->drawing_window) {
		perror("unable to create drawing window");
		raise(SIGINT);
	}
	graphics->redrawing_thread_running = 1;
	if(pthread_create(&graphics->redrawing_thread, 0, update_callback, graphics)) {
		graphics->redrawing_thread_running = 0; 
		perror("unable to create the redrawing thread\n");
		raise(SIGINT);
	}
}

u0 graphics_end(struct graphics_t *graphics) {
	SYNC(graphics->thread_lock, {graphics->redrawing_thread_running = 0;});
	
	pthread_join(graphics->redrawing_thread, 0);
	pthread_mutex_destroy(&graphics->thread_lock);	

	if(graphics->rec) {
		for(size_t i = 0; i < graphics->rec_size; i++)
			if(graphics->rec[i]) rectangle_free(&graphics->rec[i]);
		free(graphics->rec);
		graphics->rec = 0;
	}
	
	delwin(graphics->drawing_window);
}

static u8 contains_rec(struct graphics_t *graphics, struct rectangle_t *rec) {
	size_t s;
	u8 err = 0;
	SYNC(graphics->thread_lock, {s = graphics->rec_size;});
	for(size_t i = 0; i < s; i++) {
		SYNC(graphics->thread_lock, {
			if(graphics->rec[i] == rec) {
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
	SYNC(graphics->thread_lock, {
		graphics->rec[graphics->rec_size++] = rec;
	});
	u8 want_resize = graphics->rec_size == graphics->rec_cap;
	if(want_resize) {
		graphics->rec_cap *= 2;
		struct rectangle_t **next = (struct rectangle_t**)malloc(sizeof(struct rectangle_t*) * graphics->rec_cap);
		for(size_t i = 0; i < graphics->rec_cap; i++)
			next[i] = 0;
		if(!next) {
			perror("unable to malloc next buffer for regrowing\n");
			raise(SIGINT);
		}
		SYNC(graphics->thread_lock, {
			for(size_t i = 0; i < graphics->rec_size; i++) next[i] = graphics->rec[i];
			free(graphics->rec);
			graphics->rec = next;
		});
	}
}
