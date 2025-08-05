#include "rectangle.h"
#include "ncurses.h"
#include "stdlib.h"
#include "colorpairs.h"

/* for readibility */
struct rectangle_t *rectangle_malloc_with_cords(int y1, int x1, int y2, int x2) {
	struct rectangle_t *rec = (struct rectangle_t*)malloc(sizeof(struct rectangle_t));
	rec->y1 = y1;
	rec->x1 = x1;
	rec->y2 = y2;
	rec->x2 = x2;
	rec->filled = 0;
	rec->color = 0;
	rec->visibile = 0;
	return rec;
}

void rectangle_free(struct rectangle_t **recaddr) {
	free(*recaddr);
}

void rectangle_draw_border(struct rectangle_t *rec, WINDOW *win) {
	struct rectangle_t rectangle = *rec;
	COLORPAIR_ATTR(win, rec->color, {
		mvwhline(win, rectangle.y1, rectangle.x1, 0, rectangle.x2-rectangle.x1);
		mvwhline(win, rectangle.y2, rectangle.x1, 0, rectangle.x2-rectangle.x1);
		mvwvline(win, rectangle.y1, rectangle.x1, 0, rectangle.y2-rectangle.y1);
		mvwvline(win, rectangle.y1, rectangle.x2, 0, rectangle.y2-rectangle.y1);
		mvwaddch(win, rectangle.y1, rectangle.x1, ACS_ULCORNER);
		mvwaddch(win, rectangle.y2, rectangle.x1, ACS_LLCORNER);
		mvwaddch(win, rectangle.y1, rectangle.x2, ACS_URCORNER);
		mvwaddch(win, rectangle.y2, rectangle.x2, ACS_LRCORNER);
	});
}

void rectangle_draw_filled(struct rectangle_t *rec, WINDOW *win) {
	COLORPAIR_ATTR(win, rec->color, {
		for(int y = rec->y1; y < rec->y2; y++) {
			for(int x = rec->x1; x < rec->x2; x++) {
				mvwprintw(win, y, x, " ");
			}
		}
	});
}
