#include "inputloop.h"
#include "ncurses.h"
#include "bitdefs.h"
#include "threadutils.h"
#include "tetris.h"
#include "stdlib.h"

int inputloop(int (*callback)(int)) {
	while(1) {
		s32 key;
		key = getch();;
		switch(key) {
			case 'q': return callback(0);
			default: break;
		}
	}
}
