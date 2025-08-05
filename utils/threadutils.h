#ifndef THREADUTILS_H
#define THREADUTILS_H

#define SYNC(lock, block) \
do { \
	pthread_mutex_lock(&(lock)); \
	block; \
	pthread_mutex_unlock(&(lock)); \
} while (0)

#endif
