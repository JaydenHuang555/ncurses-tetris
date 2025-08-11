#include "tile.h"
#include "rectangle.h"

u0 tile_init(struct tile_t *tile, struct cords2d_t cords, enum colorpair_t colorpair) {
	tile->cords = cords;
	tile->rec = rectangle_malloc_with_cords(cords.y, cords.x, cords.y + TILE_FACE_LEN, cords.x + TILE_FACE_LEN);
	tile->rec->filled = 1;
	tile->rec->color = colorpair;
	tile->rec->visibile = 1;
}

u0 tile_publish(struct tile_t *tile, struct graphics_t *graphics) {
	graphics_register_rectangle(graphics, tile->rec);
}
