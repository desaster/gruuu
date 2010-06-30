#include <tonc.h>
#include <inttypes.h>

#include "joy.h"

#define DPAD KEY_DIR

typedef struct _joy {
	int16_t bits;

	int16_t time;
	int16_t brk;
	int16_t handled;

	JoyState dir;
} Happy;

#define J_UP(j)     ((j).bits & KEY_UP)
#define J_DOWN(j)   ((j).bits & KEY_DOWN)
#define J_LEFT(j)   ((j).bits & KEY_LEFT)
#define J_RIGHT(j)  ((j).bits & KEY_RIGHT)

static Happy joy;

#define keyStatus (~REG_KEYINPUT)

JoyState* joy_poll() {
	int timeout;

	joy.bits |= keyStatus & DPAD;

	if ((keyStatus & DPAD) == 0) {
		joy.brk = 1;
	} else {
		joy.time++;
	}

	timeout = joy.time > JOY_THRESH;

	if (joy.brk || timeout) {
		if (!joy.handled) {
			joy.dir.dy = J_UP(joy) ? -1 :
				J_DOWN(joy) ? 1 : 0;

			joy.dir.dx = J_LEFT(joy) ? -1 : 
				J_RIGHT(joy) ? 1 : 0;

			joy.dir.moving = joy.dir.dx || joy.dir.dy;
		}
		if (!timeout) {
			joy.handled = 0;
		}
		joy.time = 0;
		joy.bits = 0;
		joy.brk = 0;
	}

	return &(joy.dir);
}

void joy_init() {
	joy.dir.dx = joy.dir.dy = 0;
	joy.dir.moving = 0;
	joy.bits = 0;
	joy.time = 0;
	joy.handled = 0;
}

void joy_handled() {
	joy.dir.dx = joy.dir.dy = 0;
	joy.dir.moving = 0;
	joy.handled = 1;
	joy.dir.moving = 0;
}

/* vim: set sw=8 noet: */
