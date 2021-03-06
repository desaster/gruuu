#ifndef __RENGINE__
#define __RENGINE__

#include "genmap.h"

struct Monster {
	int x;
	int y;
	int type;
	int alive;
};

struct World {
	struct Map map[MAP_MAX_SIZE];
	struct Monster monsters[50];
	int monstercount;
};

void init_monster(struct World *world, int i);

int r_move(struct World *world, int dx, int dy);

#endif

/* vim: set sw=8 noet: */
