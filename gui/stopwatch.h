#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "time.h"
#include "bitdefs.h"

struct stopwatch_t {
    time_t start;
    s8 started;
};

extern u0 stopwatch_start(struct stopwatch_t*);
extern time_t stopwatch_getsecs(struct stopwatch_t*);

#endif
