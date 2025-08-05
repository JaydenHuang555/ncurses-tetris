#include "gui.h"
#include "rectangle.h"
#include "gameloop.h"
#include "threadutils.h"
#include "graphics.h"


static void init_drop_frame(struct gui_t *gui) {
	gui->drop_frame = rectangle_malloc_with_cords(5, 10, 30, 30); 
	gui->drop_frame->filled = 0;
	gui->drop_frame->color = COLORPAIR_DEFAULT;
	gui->drop_frame->visibile = 1;
	graphics_register_rectangle(gui->drop_frame);
}

static int runner(void *raw) {
	struct gui_t *gui = (struct gui_t*)raw;
	SYNC(gameloop_g_thread_lock, {
		gui->drop_frame->y1++;
		gui->drop_frame->y2++;
	});
	return 0;
}

void gui_init(struct gui_t *gui) {
	init_drop_frame(gui);	
	gameloop_add_runner(runner, gui);
}

void gui_deinit(void) {
}
