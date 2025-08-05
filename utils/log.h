#ifndef LOG_H
#define LOG_H

#include "string.h"
#include "stdio.h"
#include "time.h"

#ifndef LOG_OUTPUT_FILE
#define LOG_OUTPUT_FILE "log_output"
#endif

#define LOG(fmt, ...) { \
    FILE *log_file = fopen(LOG_OUTPUT_FILE, "a"); \
    if (log_file) { \
        time_t now = time(NULL); \
        struct tm *tm_info = localtime(&now); \
        char time_str[20]; \
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info); \
        fprintf(log_file, "[%s] [%s:%d] [%s] " fmt "\n", time_str, __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    } else { \
        perror("Failed to open log file"); \
    } \
}


#endif
