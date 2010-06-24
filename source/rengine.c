#include <tonc.h>

#include "rengine.h"
#include "util.h"

void init_monster(struct World *world, int i)
{
	struct Monster *monster;

	monster = &world->monsters[i];
	monster->x = 0;
	monster->y = 0;

	while (1) {
		monster->x = qran_range(0, world->map->width);
		monster->y = qran_range(0, world->map->height);
		if (world->map->mapdata[monster->x +
				(monster->y * world->map->width)] ==
				WORLD_FLOOR) {
			break;
		}
	}

	monster->type = 0;
}

/* can we walk to a specific tile on map? */
int r_can_enter(struct World *world, u32 x, u32 y)
{
	if (world->map->mapdata[x + (y * world->map->width)] != WORLD_FLOOR) {
		return 0;
	}
	return 1;
}

/* move the dude's position on map */

int r_move_left(struct World *world)
{    
	if (r_can_enter(world, world->map->dudex - 1, world->map->dudey)) {
		world->map->dudex --;
		return 1;
	}
	return 0;
}

int r_move_right(struct World *world)
{    
	if (r_can_enter(world, world->map->dudex + 1, world->map->dudey)) {
		world->map->dudex ++;
		return 1;
	}
	return 0;
}

int r_move_up(struct World *world)
{    
	if (r_can_enter(world, world->map->dudex, world->map->dudey - 1)) {
		world->map->dudey --;
		return 1;
	}
	return 0;
}

int r_move_down(struct World *world)
{    
	if (r_can_enter(world, world->map->dudex, world->map->dudey + 1)) {
		world->map->dudey ++;
		return 1;
	}
	return 0;
}

/* vim: set sw=8 noet: */