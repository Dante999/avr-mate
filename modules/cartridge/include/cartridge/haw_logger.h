#ifndef HAW_LOGGER_H
#define HAW_LOGGER_H

#define LEVEL_OFF     0
#define LEVEL_DEBUG   1
#define LEVEL_INFO    2
#define LEVEL_WARNING 3
#define LEVEL_ERROR   4

#ifndef LOG_LEVEL
#	define LOG_LEVEL LEVEL_INFO
#endif

void log_init();
void log_println(const char *prefix, const char *msg);

#if (LOG_LEVEL <= LEVEL_DEBUG)
#	define LOG_DEBUG(msg) log_println("[D] ", msg)
#else
#	define LOG_DEBUG(msg)
#endif

#if (LOG_LEVEL <= LEVEL_INFO)
#	define LOG_INFO(msg) log_println("[I] ", msg)
#else
#	define LOG_INFO(msg)
#endif

#if (LOG_LEVEL <= LEVEL_WARNING)
#	define LOG_WARNING(msg) log_println("[W] ", msg)
#else
#	define LOG_WARNING(msg)
#endif

#if (LOG_LEVEL <= LEVEL_ERROR)
#	define LOG_ERROR(msg) log_println("[E] ", msg)
#else
#	define LOG_ERROR(msg)
#endif

#endif // HAW_LOGGER_H
