#include "tile.h"
#include "rectangle.h"

u0 tile_init(struct tile_t *tile, struct cords2d_t cords, enum colorpair_t colorpair) {
	tile->cords = cords;
	struct cords2d_t top_left = cords, bottom_right = {.y = cords.y + TILE_FACE_HEIGHT, .x = cords.x + TILE_FACE_WIDTH};
	tile->rec = rectangle_malloc_with_cords(top_left, bottom_right);
	tile->rec->filled = 1;
	tile->rec->color = colorpair;
	tile->rec->visibile = 1;
}

u0 tile_publish(struct tile_t *tile, struct graphics_t *graphics) {
	graphics_register_rectangle(graphics, tile->rec);
}
