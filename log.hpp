#ifndef LOG_HPP
#define LOG_HPP

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#ifdef __cplusplus
}
#endif

#define log(fmt, ...) printf( ("INFO [%s:%d]: " fmt "\n"), __FILE__, __LINE__, __VA_ARGS__);

#ifdef DEBUG
#define log_debug(fmt, ...) printf(("DEBUG [%s:%d]: " fmt "\n"), __FILE__, __LINE__, __VA_ARGS__);
#else
#define log_debug(...);
#endif
#endif

/*
#define log(fmt, ...) printf(("[%s:%d] %s: " fmt "\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);

#ifdef DEBUG
#       define log_debug(fmt, ...) printf(("[%s:%d] %s: " fmt "\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#else
#define log(...)
#endif
*/
