#ifndef BALL_H
#define BALL_H

#include <stdint.h>

enum direction {
	DIR_UP,
	DIR_UPLEFT,
	DIR_UPRIGHT,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_DOWNLEFT,
	DIR_DOWNRIGHT
};

struct ball {
	uint8_t        x;
	uint8_t        y;
	uint8_t        min_x;
	uint8_t        max_x;
	uint8_t        min_y;
	uint8_t        max_y;
	enum direction dir;
};

void ball_bounce(struct ball *ball);

#endif // BALL_H
