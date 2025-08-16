#include "gui.h"
#include "unistd.h"
#include "tile.h"
#include <stdlib.h>
#include "rectangle.h"
#include "gameloop.h"
#include "graphics.h"
#include "tetris.h"
#include "threadutils.h"

static void init_drop_frame(struct gui_t *gui) {
	struct cords2d_t top_left = {.y = 5, .x = 10}, bottom_right = {.y = 40, .x = 50};
	gui->drop_frame = rectangle_malloc_with_cords(top_left, bottom_right);
	gui->drop_frame->filled = 0;
	gui->drop_frame->color = 1;
	gui->drop_frame->visibile = 1;
	graphics_register_rectangle(&graphics, gui->drop_frame);
}

static void init_help_frame(struct gui_t *gui) {
	struct cords2d_t top_left = {.y = 5, .x = 51}, bottom_right = {.y = 20, .x = 71};
	gui->next_block_frame = rectangle_malloc_with_cords(top_left, bottom_right);
	gui->next_block_frame->filled = 0;
	gui->next_block_frame->color = 1;
	gui->next_block_frame->visibile = 1;
	graphics_register_rectangle(&graphics, gui->next_block_frame);
}

static void create_tile(struct gui_t *gui, struct cords2d_t cords, enum colorpair_t colorpair) {
	struct tile_t *tile = malloc(sizeof(struct tile_t));
	tile_init(tile, cords, colorpair);
	gui->tile_info.tiles[gui->tile_info.offset++] = tile;
	if(gui->tile_info.offset == gui->tile_info.len) {
		struct tile_t **next = (struct tile_t**)malloc(sizeof(struct tile_t*) * (gui->tile_info.len *= 2));
		for(size_t i = 0; i < gui->tile_info.len; i++)
			next[i] = i < gui->tile_info.offset / 2 ? gui->tile_info.tiles[i] : 0;
		free(gui->tile_info.tiles);
		gui->tile_info.tiles = next;
	}
	tile_publish(tile, &graphics);
}

static int runner(void *raw) {
	struct gui_t *gui = (struct gui_t*)raw;
	SYNC(gui->mutex, {
		for(size_t i = 0; i < gui->tile_info.offset; i++) {
			struct tile_t *tile = gui->tile_info.tiles[i];
			s32 tile_colide = (tile->rec->cords_bottom_right.y == gui->drop_frame->cords_bottom_right.y);
			if(!tile_colide && stopwatch_getsecs(&gui->stopwatch) >= 1) {
				tile->rec->cords_top_left.y++;
				tile->rec->cords_bottom_right.y++;
				stopwatch_start(&gui->stopwatch);
			}
		}
	});
	return 0;
}

void gui_init(struct gui_t *gui) {
	pthread_mutex_init(&gui->mutex, 0);
	gui->stopwatch.start = 0;
	gui->stopwatch.started = 0;
	gui->tile_info.len = 1 << 4;
	gui->tile_info.offset = 0;
	gui->tile_info.tiles = (struct tile_t**)malloc(sizeof(struct tile_t*) * gui->tile_info.len);
	for(size_t i = 0; i < gui->tile_info.len; i++) gui->tile_info.tiles[i] = 0;
	init_drop_frame(gui);
	init_help_frame(gui);
	struct cords2d_t cords = {.y = 6, .x = 11};
	atomic_store(&gui->running, 1);
	gameloop_add_runner(&gameloop, runner, gui);
	create_tile(gui, cords, COLORPAIR_RED);
	stopwatch_start(&gui->stopwatch);
}

void gui_deinit(struct gui_t *gui) {
	atomic_store(&gui->running, 0);
	for(size_t i = 0; i < gui->tile_info.offset; i++)
		free(gui->tile_info.tiles[i]);
	free(gui->tile_info.tiles);
	pthread_mutex_destroy(&gui->mutex);
}
