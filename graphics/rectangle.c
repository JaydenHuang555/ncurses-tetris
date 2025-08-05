/*

TODO: have my auto indent use proper indenting on this file

#include "rectangle.h"
#include "ncurses.h"
#include "stdlib.h"
#include "colorpairs.h"

struct rectangle_t *rectangle_malloc_with(int y1, int x1, int y2, int x2) {
	struct rectangle_t *rec = (struct rectangle_t*)malloc(sizeof(struct rectangle_t));
	rec->y1 = y1;
	rec->x1 = x1;
	rec->y2 = y2;
	rec->x2 = x2;
	rec->filled = 0;
	return rec;
}

void rectangle_free(struct rectangle_t **recaddr) {
	free(*recaddr);
}

void rectangle_draw_border(struct rectangle_t *rec, WINDOW *win) {
	struct rectangle_t rectangle = *rec;
	mvwhline(win, rectangle.y1, rectangle.x1, 0, rectangle.x2-rectangle.x1);
	mvwhline(win, rectangle.y2, rectangle.x1, 0, rectangle.x2-rectangle.x1);
	mvwvline(win, rectangle.y1, rectangle.x1, 0, rectangle.y2-rectangle.y1);
	mvwvline(win, rectangle.y1, rectangle.x2, 0, rectangle.y2-rectangle.y1);
	mvwaddch(win, rectangle.y1, rectangle.x1, ACS_ULCORNER);
	mvwaddch(win, rectangle.y2, rectangle.x1, ACS_LLCORNER);
	mvwaddch(win, rectangle.y1, rectangle.x2, ACS_URCORNER);
	mvwaddch(win, rectangle.y2, rectangle.x2, ACS_LRCORNER);
}

void rectangle_draw_filled(struct rectangle_t *rec, WINDOW *win) {
	// rectangle_draw_border(rec, win);
	// offset by 1 to allow for border
    init_pair(1, COLOR_GREEN, COLOR_RED);
	wattron(win ,COLOR_PAIR(1));
	for(int y = rec->y1; y < rec->y2; y++) {
		for(int x = rec->x1; x < rec->x2; x++) {
			mvwprintw(win, y, x, " ");
		}
	}
	wrefresh(win);
	wattroff(win, COLOR_PAIR(1));
}
*/
