#ifndef SMALLOC_H
#define SMALLOC_H

#ifndef MALLOC_H
#err "please define malloc"
#endif

#include "stddef.h"

void *smmalloc(size_t size, int sig, const char *fmt, ...);

void *smalloc(size_t size, int sig);

void *sismmalloc(size_t size, const char *fmt);
void *sismalloc(size_t size);

#endif
