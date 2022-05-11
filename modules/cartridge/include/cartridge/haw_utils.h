#ifndef HAW_UTILS_H
#define HAW_UTILS_H

#ifdef __AVR__
#	include <util/delay.h>
#else
#	include <unistd.h>
#endif

#ifdef __AVR__
#	define sleep_for_ms(x) _delay_ms(x)
#else
#	define sleep_for_ms(x) usleep(x * 1000)
#endif

#endif // HAW_UTILS_H
