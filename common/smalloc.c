#include "malloc.h"
#include "signal.h"
#include "stdarg.h"
#include "stdio.h"

void *smmalloc(size_t size, int sig, const char *fmt, ...) {
	void *block = malloc(size);	
	if(!block) {
		if(fmt) {
			va_list ap;
			va_start(ap, fmt);
			vfprintf(stderr, fmt, ap);	
			va_end(ap);
		}
		raise(sig);
	}
	return block;
}

void *smalloc(size_t size, int sig) {
	return smmalloc(size, sig, 0);
}

void *sismmalloc(size_t size, const char *fmt) {
	return smmalloc(size, SIGINT, fmt);	
}

void *sismalloc(size_t size) {
	return smalloc(size, SIGINT);
}

