#include "ncurses.h"
#include <signal.h>
#include "loops/gameloop.h"
#include "loops/inputloop.h"
#include "graphics/graphics.h"
#include "log.h"
#include "bitdefs.h"
#include "gui/gui.h"
#include "threadutils.h"

#ifndef __linux__
#warning "compiling for untested env"
#endif

#ifdef __WIN32
#err "does not compile for windows, please contact @JaydenHuang555 to send u an windows copy"
#endif

pthread_mutex_t ncurses_mutex = PTHREAD_MUTEX_INITIALIZER;

static struct gui_t gui;
struct graphics_t graphics;
struct gameloop_t gameloop;

static int handle_end(s32 exit_code) {
	graphics_end(&graphics);
	gui_deinit(&gui);
	gameloop_end(&gameloop);
	endwin();
	return exit_code;
}

static void handle_sigint(s32 catch) {
	handle_end(catch * 0 + 139);
}

static u0 start_scrn(u0) {
	initscr();
	noecho();
	keypad(stdscr, 1);
	cbreak();
	if(has_colors()) {
		start_color();
	}

	clear();
	graphics_start(&graphics, &ncurses_mutex);
	refresh();
	gameloop_start(&gameloop);

	gui_init(&gui);
}

s32 main(u0) {
	start_scrn();
	signal(SIGINT, handle_sigint);	
	s32 exit = inputloop(handle_end);
	pthread_mutex_destroy(&ncurses_mutex);
	return exit;
}
