#include "rectangle.h"
#include "ncurses.h"
#include "stdlib.h"
#include "colorpairs.h"

struct rectangle_t *rectangle_malloc_with_indv_cords(int y1, int x1, int y2, int x2) {
	struct rectangle_t *rec = malloc(sizeof(struct rectangle_t));
	rec->cords_top_left.y = y1;
	rec->cords_top_left.x = x1;
	rec->cords_bottom_right.y = y2;
	rec->cords_bottom_right.x = x2;
	rec->filled = 0;
	rec->color = COLORPAIR_WHITE;
	rec->visibile = 0;
	return rec;
}

struct rectangle_t *rectangle_malloc_with_cords(struct cords2d_t top_left, struct cords2d_t bottom_right) {
	struct rectangle_t *rec = malloc(sizeof(struct rectangle_t));
	rec->cords_top_left = top_left;
	rec->cords_bottom_right = bottom_right;
	rec->filled = 0;
	rec->color = COLORPAIR_DEFAULT;
	rec->visibile = 0;
	return rec;
}

struct rectangle_t *rectangle_malloc_with_dims(struct cords2d_t top_left, s32 height, s32 width) {
	struct rectangle_t *rec = malloc(sizeof(struct rectangle_t));
	rec->cords_top_left = top_left;
	rec->cords_bottom_right.y = top_left.y + height;
	rec->cords_bottom_right.x = top_left.x + width;
	return rec;
}

void rectangle_free(struct rectangle_t **recaddr) {
	free(*recaddr);
}

// void rectangle_draw_border(struct rectangle_t *rec, WINDOW *win) {
// 	struct rectangle_t rectangle = *rec;
// 	COLORPAIR_ATTR(win, rec->color, {
// 		mvwhline(win, rectangle.cords_top_left.y, rectangle.cords_top_left.x, 0, rectangle.cords_bottom_right.x-rectangle.cords_top_left.x);
// 		mvwhline(win, rectangle.cords_bottom_right.y, rectangle.cords_top_left.x, 0, rectangle.cords_bottom_right.x-rectangle.cords_top_left.x);
// 		mvwvline(win, rectangle.cords_top_left.y, rectangle.cords_top_left.x, 0, rectangle.cords_bottom_right.y-rectangle.cords_top_left.y);
// 		mvwvline(win, rectangle.cords_top_left.y, rectangle.cords_bottom_right.x, 0, rectangle.cords_bottom_right.y-rectangle.cords_top_left.y);
// 		mvwaddch(win, rectangle.cords_top_left.y, rectangle.cords_top_left.x, ACS_ULCORNER);
// 		mvwaddch(win, rectangle.cords_bottom_right.y, rectangle.cords_top_left.x, ACS_LLCORNER);
// 		mvwaddch(win, rectangle.cords_top_left.y, rectangle.cords_bottom_right.x, ACS_URCORNER);
// 		mvwaddch(win, rectangle.cords_bottom_right.y, rectangle.cords_bottom_right.x, ACS_LRCORNER);
// 	});
// }

u0 rectangle_draw_border(struct rectangle_t *rec, WINDOW *win) {
	COLORPAIR_ATTR(win, rec->color, {
		// set up top row
		for(int x = rec->cords_top_left.x; x < rec->cords_bottom_right.x; x++)
			mvwaddch(win, rec->cords_top_left.y, x, ' ');

		// set up sides
		for(int y = rec->cords_top_left.y; y < rec->cords_bottom_right.y; y++) {
			mvwaddch(win, y, rec->cords_top_left.x, ' ');
			mvwaddch(win, y, rec->cords_bottom_right.x, ' ');
		}
		
		// set up bottom row
		for(int x = rec->cords_top_left.x; x < rec->cords_bottom_right.x + 1; x++)
			mvwaddch(win, rec->cords_bottom_right.y, x, ' ');

	});
}

void rectangle_draw_filled(struct rectangle_t *rec, WINDOW *win) {
	COLORPAIR_ATTR(win, rec->color, {
		for(int y = rec->cords_top_left.y; y < rec->cords_bottom_right.y; y++) {
			for(int x = rec->cords_top_left.x; x < rec->cords_bottom_right.x; x++) {
				mvwprintw(win, y, x, " ");
			}
		}
	});
}
