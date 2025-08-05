#include "ncurses.h"
#include <signal.h>
#include "loops/gameloop.h"
#include "loops/inputloop.h"
#include "graphics/graphics.h"
#include "log.h"
#include "bitdefs.h"
#include "gui/gui.h"
#include "threadutils.h"

pthread_mutex_t ncurses_mutex = PTHREAD_MUTEX_INITIALIZER;

static struct gui_t gui;

static int handle_end(s32 exit_code) {
	graphics_end();
	gui_deinit();
	gameloop_end();
	endwin();
	return exit_code;
}

static void handle_sigint(s32 catch) {
	handle_end(139);
}

static u0 start_scrn(u0) {
	// LOG("---------------------------------------------------------------------------- STARTING SCREEN ---------------------------------------------------------------------------- ");
	initscr();
	noecho();
	keypad(stdscr, 1);
	cbreak();
	if(has_colors()) {
		start_color();
	}

	clear();
	graphics_start(&ncurses_mutex);
	refresh();
	gameloop_start();

	gui_init(&gui);
	// rectangle_draw(rec);
}

s32 main(s32 argc, s8 **argv) {
	start_scrn();
	signal(SIGINT, handle_sigint);	
	s32 exit = inputloop(handle_end);
	pthread_mutex_destroy(&ncurses_mutex);
	return exit;
}
