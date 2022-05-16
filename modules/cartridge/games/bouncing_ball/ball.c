#include "ball.h"

#include <stdbool.h>

static bool poke_up(struct ball *ball)
{
	if (ball->y > ball->min_y) {
		ball->y--;
		return true;
	}
	else {
		ball->y++;
		return false;
	}
}

static bool poke_down(struct ball *ball)
{
	if (ball->y < ball->max_y) {
		ball->y++;
		return true;
	}
	else {
		ball->y--;
		return false;
	}
}

static bool poke_left(struct ball *ball)
{
	if (ball->x > ball->min_x) {
		ball->x--;
		return true;
	}
	else {
		ball->x++;
		return false;
	}
}

static bool poke_right(struct ball *ball)
{
	if (ball->x < ball->max_x) {
		ball->x++;
		return true;
	}
	else {
		ball->x--;
		return false;
	}
}

static void move_up(struct ball *ball)
{
	if (!poke_up(ball)) {
		ball->dir = DIR_DOWN;
	}
}

static void move_down(struct ball *ball)
{
	if (!poke_down(ball)) {
		ball->dir = DIR_UP;
	}
}

static void move_left(struct ball *ball)
{
	if (!poke_left(ball)) {
		ball->dir = DIR_RIGHT;
	}
}
static void move_right(struct ball *ball)
{
	if (!poke_right(ball)) {
		ball->dir = DIR_LEFT;
	}
}

static void move_upleft(struct ball *ball)
{
	bool hit_top  = !poke_up(ball);
	bool hit_left = !poke_left(ball);

	if (hit_top && hit_left) {
		ball->dir = DIR_DOWNRIGHT;
	}
	else if (hit_top) {
		ball->dir = DIR_DOWNLEFT;
	}
	else if (hit_left) {
		ball->dir = DIR_UPRIGHT;
	}
}

static void move_upright(struct ball *ball)
{
	bool hit_top   = !poke_up(ball);
	bool hit_right = !poke_right(ball);

	if (hit_top && hit_right) {
		ball->dir = DIR_DOWNLEFT;
	}
	else if (hit_top) {
		ball->dir = DIR_DOWNRIGHT;
	}
	else if (hit_right) {
		ball->dir = DIR_UPLEFT;
	}
}

static void move_downleft(struct ball *ball)
{
	bool hit_bot  = !poke_down(ball);
	bool hit_left = !poke_left(ball);

	if (hit_bot && hit_left) {
		ball->dir = DIR_UPRIGHT;
	}
	else if (hit_bot) {
		ball->dir = DIR_UPLEFT;
	}
	else if (hit_left) {
		ball->dir = DIR_DOWNRIGHT;
	}
}

static void move_downright(struct ball *ball)
{
	bool hit_bot   = !poke_down(ball);
	bool hit_right = !poke_right(ball);

	if (hit_bot && hit_right) {
		ball->dir = DIR_UPLEFT;
	}
	else if (hit_bot) {
		ball->dir = DIR_UPRIGHT;
	}
	else if (hit_right) {
		ball->dir = DIR_DOWNLEFT;
	}
}

void ball_bounce(struct ball *ball)
{
	switch (ball->dir) {

	case DIR_UP:
		move_up(ball);
		break;

	case DIR_DOWN:
		move_down(ball);
		break;

	case DIR_LEFT:
		move_left(ball);
		break;

	case DIR_RIGHT:
		move_right(ball);
		break;

	case DIR_UPLEFT:
		move_upleft(ball);
		break;

	case DIR_UPRIGHT:
		move_upright(ball);
		break;

	case DIR_DOWNLEFT:
		move_downleft(ball);
		break;

	case DIR_DOWNRIGHT:
		move_downright(ball);
		break;
	}
}
