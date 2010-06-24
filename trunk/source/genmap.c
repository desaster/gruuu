#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <tonc.h>

#include "util.h"
#include "map.h"

#define COINFLIP() (qran_range(0, 1) == 0)

struct Mapcell
{
	/* X and Y within the map */
	u32 x;
	u32 y;
	u8 used;
};

struct Room
{
	/* X and Y within the map */
	u32 x;
	u32 y;
	u32 width;
	u32 height;
};

void drawroom(struct Map *map, struct Room *room)
{
	u32 x, y;

	for (x = 0; x < room->width; x ++) {
		map->mapdata[(x + room->x) + (room->y * map->width)] =
			WORLD_WALL;
		map->mapdata[(x + room->x) +
			((room->y + room->height - 1) * map->width)] =
			WORLD_WALL;
	}
	for (y = 0; y < room->height; y ++) {
		map->mapdata[room->x + ((room->y + y) * map->width)] =
			WORLD_WALL;
		map->mapdata[(room->x + room->width - 1) +
			((room->y + y) * map->width)] = WORLD_WALL;
	}
	for (y = 1; y < room->height - 1; y ++) {
		for (x = 1; x < room->width - 1; x ++) {
			map->mapdata[(x + room->x) +
				((y + room->y) * map->width)] =
				WORLD_FLOOR;
		}
	}
}

void initmap(struct Map *map, u32 width, u32 height)
{
	u32 i;
	u8 mapdata[MAP_MAX_SIZE];
	/* struct Map *map = (struct Map *) malloc(sizeof(struct Map)); */
	/* struct Map map[MAP_MAX_SIZE]; */

	/* map->mapdata = (u8 *) malloc(sizeof(u8) * width * height); */
	map->mapdata = mapdata;
	map->width = width;
	map->height = height;
	map->dudex = 0;
	map->dudey = 0;

	for (i = 0; i < width * height; i ++) {
		map->mapdata[i] = WORLD_NOTHING;
	}

	/* return map; */
}

void genroom(struct Mapcell *cell, struct Room *room)
{
	u32 r;
	/* struct Room room; */

	/* room = (struct Room *) malloc(sizeof(struct Room)); */

	/* Some space around the room */
	room->x = cell->x + 1;
	room->y = cell->y + 1;
	room->height = MAP_CELL_HEIGHT - 1;
	room->width = MAP_CELL_WIDTH - 1;

	r = qran_range(0, MAP_CELL_WIDTH) / 2;
	if (COINFLIP()) room->x += r;
	room->width -= r;

	r = qran_range(0, MAP_CELL_HEIGHT) / 2;
	if (COINFLIP()) room->y += r;
	room->height -= r;

	/* return &room; */
}

/* Create walls on empty tiles all around us */
void isolate(struct Map *map, u32 x, u32 y)
{
	u32 dirs[8], i = 0;

	dirs[i ++] = x + ((y - 1) * map->width); /* north */
	dirs[i ++] = x + ((y + 1) * map->width); /* south */
	dirs[i ++] = (x - 1 + (y * map->width)); /* west */
	dirs[i ++] = (x + 1 + (y * map->width)); /* east */
	dirs[i ++] = x - 1 + ((y - 1) * map->width); /* northwest */
	dirs[i ++] = x + 1 + ((y - 1) * map->width); /* northeast */
	dirs[i ++] = (x - 1 + ((y + 1) * map->width)); /* southwest */
	dirs[i ++] = (x + 1 + ((y + 1) * map->width)); /* southeast */

	for (i = 0; i < 8; i ++) {
		if (i > (map->height * map->width)) continue;
		if (map->mapdata[dirs[i]] == WORLD_NOTHING) {
			map->mapdata[dirs[i]] = WORLD_WALL;
		}
	}
}

/* Horizontal tunnel. This actually doesn't go all the way to x2, but that's
 * fine, because we end up in the middle of a room anyway */
void drawtunnel_h(struct Map *map, u32 x1, u32 x2, u32 y)
{
	u32 x, wup, wdown;
	for (x = x1; x != x2; x += (x1 < x2 ? 1 : -1)) {
		wup = x + ((y - 1) * map->width);
		wdown = x + ((y + 1) * map->width);
		if (map->mapdata[wup] != WORLD_FLOOR) {
			map->mapdata[wup] = WORLD_WALL;
		}
		if (map->mapdata[wdown] != WORLD_FLOOR) {
			map->mapdata[wdown] = WORLD_WALL;
		}
		map->mapdata[x + (y * map->width)] = WORLD_FLOOR;
	}
	map->mapdata[x + (y * map->width)] = WORLD_FLOOR;
	isolate(map, x, y);
}

/* Vertical tunnel. This actually doesn't go all the way to y2, but that's
 * fine, because we end up in the middle of a room anyway */
void drawtunnel_v(struct Map *map, u32 y1, u32 y2, u32 x)
{
	u32 y, wleft, wright;
	for (y = y1; y != y2; y += (y1 < y2 ? 1 : -1)) {
		wleft = (x - 1 + (y * map->width));
		wright = (x + 1 + (y * map->width));
		if (map->mapdata[wleft] != WORLD_FLOOR) {
			map->mapdata[wleft] = WORLD_WALL;
		}
		if (map->mapdata[wright] != WORLD_FLOOR) {
			map->mapdata[wright] = WORLD_WALL;
		}
		map->mapdata[x + (y * map->width)] = WORLD_FLOOR;
	}
	isolate(map, x, y);
}

void drawpath(struct Map *map, struct Room *room1, struct Room *room2)
{
	u32 cx1, cy1, cx2, cy2;

	cx1 = room1->x + (room1->width / 2);
	cy1 = room1->y + (room1->height / 2);
	cx2 = room2->x + (room2->width / 2);
	cy2 = room2->y + (room2->height / 2);

	drawtunnel_h(map, cx1, cx2, cy1);
	drawtunnel_v(map, cy1, cy2, cx2);
}

void genmap(struct Map *map, u32 width, u32 height)
{
	u32 i, numrooms, x, y;
	initmap(map, width, height);
	u32 cellsx = map->width / MAP_CELL_WIDTH;
	u32 cellsy = map->height / MAP_CELL_HEIGHT;
	struct Mapcell mapcells[cellsx * cellsy], *cell;
	struct Room rooms[cellsx * cellsy]; 
	/* struct Room rooms[500]; */

	for (y = 0; y < cellsy; y ++) {
		for (x = 0; x < cellsx; x ++) {
			cell = &mapcells[x + (y * cellsx)];
			cell->x = (map->width / MAP_CELL_WIDTH * MAP_CELL_WIDTH) /
				cellsx * x;
			cell->y = (map->height / MAP_CELL_HEIGHT * MAP_CELL_HEIGHT) /
				cellsy * y;
			cell->used = 0;
		}
	}

	/* room density 40-60 percent */
	numrooms = (cellsx * cellsy) * qran_range(40, 60) / 100;
	for (i = 0; i != numrooms;) {
		cell = &mapcells[qran_range(0, cellsx * cellsy)];
		if (cell->used) {
			continue;
		}
		genroom(cell, &rooms[i++]);

		cell->used = 1;
	}

	/* use this to test cell placement */
	/*
	for (i = 0; i < cellsx * cellsy; i ++) {
		drawroom(map, genroom(&mapcells[i]));
	}
	*/


	for (i = 0; i < numrooms; i ++) {
		drawroom(map, &rooms[i]);
	}

	for (i = 0; i < numrooms - 1; i ++) {
		drawpath(map, &rooms[i], &rooms[i + 1]);
	}

	/* Give rooms a number */
	/*
	for (i = 0; i < numrooms; i ++) {
		u8 c[2];
		sprintf(c, "%d", i + 1);
		map->mapdata[(rooms[i]->x + rooms[i]->width / 2) +
			((rooms[i]->y + rooms[i]->height / 2) *
			map->width)] = c[0];
	}
	*/

	/*
	for (i = 0; i < numrooms; i ++) free(rooms[i]);
	*/

	/* FIXME: dude should be placed in a room */
	while (1) {
		map->dudex = qran_range(0, map->width);
		map->dudey = qran_range(0, map->height);
		if (map->mapdata[map->dudex + (map->dudey * map->width)] ==
				WORLD_FLOOR) {
			break;
		}
	}

	/* return map; */
}

void map_free(struct Map *map)
{
	/*
	free(map->mapdata);
	free(map);
	*/
}

/* vim: set sw=8 noet: */
