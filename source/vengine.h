#ifndef __VENGINE__
#define __VENGINE__

#include <tonc.h>

#include "rengine.h"
#include "map.h"

#define COLOR_BLACK		0
#define COLOR_BLACK_BOLD	1
#define COLOR_RED		2
#define COLOR_RED_BOLD		3
#define COLOR_GREEN		4
#define COLOR_GREEN_BOLD	5
#define COLOR_YELLOW		6
#define COLOR_YELLOW_BOLD	7
#define COLOR_BLUE		8
#define COLOR_BLUE_BOLD		9
#define COLOR_MAGENTA		10
#define COLOR_MAGENTA_BOLD	11
#define COLOR_CYAN		12
#define COLOR_CYAN_BOLD		13
#define COLOR_WHITE		14
#define COLOR_WHITE_BOLD	15

#define CHARTILE(x) (u16 *) &(charset3Tiles)[x * 8 * 8]
#define CHARTILELEN 8 * 8 * 16

#define SPR_DUDE &veng->obj_buffer[veng->sprites[veng->spr_dude].sprite]
#define SPR_MOB(idx) &veng->obj_buffer[veng->monsters[idx].sprite]

struct VEngine_Sprite {
	u8 sprite;
	u16 tile;
	u8 allocated;
};

struct VEngine_Monster {
	u8 sprite; /* indexes to a sprite */
	u32 mobposx;
	u32 mobposy;
	u8 active;
};

struct VEngine {
	/* Position of the dude on visible screen */
	u32 dudeposx;
	u32 dudeposy;

	/* Offset in tile sizes at where to display the tilemap */
	u32 mapofsx;
	u32 mapofsy;

	/* Sprites go here */
	OBJ_ATTR obj_buffer[128];
	struct VEngine_Sprite sprites[128];
	u8 tiles_allocated[128]; /* sprite tiles */
	u8 sprites_allocated[128];

	u8 spr_dude; /* index in sprites */
	struct VEngine_Monster monsters[50];
};

void v_init(struct VEngine *veng);

void v_init_monster(struct VEngine *veng, u8 idx);

void v_shake_dude(struct VEngine *veng, u8 x, u8 y);

u8 v_tile_alloc(struct VEngine *veng);
void v_tile_free(struct VEngine *veng, u8 idx);
u8 v_spr_alloc(struct VEngine *veng);
void v_spr_free(struct VEngine *veng, u8 idx);
int v_spr_create(struct VEngine *veng, u16 *tile, u16 color);

void v_putbgtile(int x, int y, int type);
void v_draw_bg(struct Map *map, struct VEngine *veng, int dudex, int dudey);
void v_draw_more_bg(
	struct Map *map,
	int fromx, int fromy,
	int cols, int lines,
	int mapx, int mapy);

void v_draw_dude(struct VEngine *veng);
void v_draw_monster(struct VEngine *veng, struct World *world, u8 idx);
void v_move(struct VEngine *veng, int dx, int dy);
int v_move_co(struct VEngine *veng);
int v_at_edge(struct VEngine *veng, struct World *world);
void v_scroll_at_edge(struct VEngine *veng, struct World *world);

#endif

/* vim: set sw=8 noet: */
