#ifndef __MAP__
#define __MAP__

#define WORLD_NOTHING 0
#define WORLD_WALL 1
#define WORLD_FLOOR 2
#define WORLD_DOOR 3

#define MAP_MAX_WIDTH 100
#define MAP_MAX_HEIGHT 100
#define MAP_MAX_SIZE MAP_MAX_WIDTH * MAP_MAX_HEIGHT
#define MAP_CELL_WIDTH 10
#define MAP_CELL_HEIGHT 10

struct Map
{
	u32 height;
	u32 width;
	u32 dudex;
	u32 dudey;
	u8 *mapdata;
};

#endif

/* vim: set sw=8 noet: */
