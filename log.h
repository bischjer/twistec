#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define log(fmt, ...) printf( ("INFO [%s:%d]: " fmt "\n"), __FILE__, __LINE__, ##__VA_ARGS__);
#ifdef DEBUG
#define log_debug(fmt, ...) printf(("DEBUG [%s:%d]: " fmt "\n"), __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define log_debug(...);

#endif

#endif
