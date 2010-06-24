#ifndef __RENGINE__
#define __RENGINE__

#include "genmap.h"

struct Monster {
	int x;
	int y;
	int type;
};

struct World {
	struct Map map[MAP_MAX_SIZE];
	struct Monster monsters[30];
	int monstercount;
};

void init_monster(struct World *world, int i);

int r_move_left(struct World *world);
int r_move_right(struct World *world);
int r_move_up(struct World *world);
int r_move_down(struct World *world);

#endif

/* vim: set sw=8 noet: */
