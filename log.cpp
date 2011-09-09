#ifndef log
#       define log(fmt, ...) printf(("[%s:%d] %s: " fmt "\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#endif

#ifdef DEBUG
#       define log_debug(fmt, ...) printf(("[%s:%d] %s: " fmt "\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#else
#       define log_debug(...)
#endif
