#include "cartridge/haw_logger.h"

#include <stdio.h>

void log_print(const char *msg)
{
#if LOG_LEVEL > LEVEL_OFF
	printf("%s", msg);
#endif
}

void log_println(const char *prefix, const char *msg)
{
#if LOG_LEVEL > LEVEL_OFF
	printf("%s%s\n", prefix, msg);
#endif
}
