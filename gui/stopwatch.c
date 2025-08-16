#include "stopwatch.h"

u0 stopwatch_start(struct stopwatch_t *stopwatch) {
	stopwatch->started = 1;
	stopwatch->start = time(0);
}
time_t stopwatch_getsecs(struct stopwatch_t *stopwatch) {
	return time(0) - stopwatch->start;	
}
