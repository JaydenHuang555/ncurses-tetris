#ifndef IOUTILS_H
#define IOUTILS_H

#define fprintln(stream, x, ...)\
    fprintf(stream, x, ##__VA_ARGS__);\
    putc('\n', stream);\

#define eprintf(x, ...) fprintf(stderr, x, ##__VA_ARGS__)
#define eprintln(x, ...) fprintln(stderr, x, ##__VA_ARGS__)

#define println(x, ...) fprintln(stdout, x, ##__VA_ARGS__)

#endif
