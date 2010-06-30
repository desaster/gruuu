#ifndef _JOY_H
#define _JOY_H

#include <inttypes.h>

#define JOY_THRESH  8

typedef struct _jstate {
	int16_t dx;
	int16_t dy;
	int16_t moving;
} JoyState;

void joy_init();

JoyState* joy_poll();

void joy_handled();

#endif

/* vim: set sw=8 noet: */
