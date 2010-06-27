#include <string.h>

#include "charset3.h"
#include "vengine.h"
#include "util.h"
#include "map.h"

/* Get the screen entry index for a tile-coord pair
 * And yes, the div and mods will be converted by the compiler (tonc) */
u32 se_index(u32 tx, u32 ty, u32 pitch)
{	
	u32 sbb = (ty / 32) * (pitch / 32) + (tx / 32);
	return sbb * 1024 + (ty % 32) * 32 + tx % 32;
}

/* Colors taken from putty */
void init_palette()
{
	pal_bg_mem[COLOR_BLACK] =		RGB15(0, 0, 0);
	pal_bg_mem[COLOR_BLACK_BOLD] =		RGB15(21, 21, 21);
	pal_bg_mem[COLOR_RED] =			RGB15(27, 0, 0);
	pal_bg_mem[COLOR_RED_BOLD] =		RGB15(31, 21, 21);
	pal_bg_mem[COLOR_GREEN] =		RGB15(0, 27, 0);
	pal_bg_mem[COLOR_GREEN_BOLD] =		RGB15(21, 31, 21);
	pal_bg_mem[COLOR_YELLOW] =		RGB15(27, 27, 0);
	pal_bg_mem[COLOR_YELLOW_BOLD] =		RGB15(31, 31, 21);
	pal_bg_mem[COLOR_BLUE] =		RGB15(0, 0, 27);
	pal_bg_mem[COLOR_BLUE_BOLD] =		RGB15(21, 21, 31);
	pal_bg_mem[COLOR_MAGENTA] =		RGB15(27, 0, 27);
	pal_bg_mem[COLOR_MAGENTA_BOLD] =	RGB15(31, 21, 31);
	pal_bg_mem[COLOR_CYAN] =		RGB15(0, 27, 27);
	pal_bg_mem[COLOR_CYAN_BOLD] =		RGB15(21, 31, 31);
	pal_bg_mem[COLOR_WHITE] =		RGB15(27, 27, 27);
	pal_bg_mem[COLOR_WHITE_BOLD] =		RGB15(31, 31, 31);

	pal_obj_mem[1 + COLOR_BLACK * 16] =		RGB15(0, 0, 0);
	pal_obj_mem[1 + COLOR_BLACK_BOLD * 16] =	RGB15(21, 21, 21);
	pal_obj_mem[1 + COLOR_RED * 16] =		RGB15(27, 0, 0);
	pal_obj_mem[1 + COLOR_RED_BOLD * 16] =		RGB15(31, 21, 21);
	pal_obj_mem[1 + COLOR_GREEN * 16] =		RGB15(0, 27, 0);
	pal_obj_mem[1 + COLOR_GREEN_BOLD * 16] =	RGB15(21, 31, 21);
	pal_obj_mem[1 + COLOR_YELLOW * 16] =		RGB15(27, 27, 0);
	pal_obj_mem[1 + COLOR_YELLOW_BOLD * 16] =	RGB15(31, 31, 21);
	pal_obj_mem[1 + COLOR_BLUE * 16] =		RGB15(0, 0, 27);
	pal_obj_mem[1 + COLOR_BLUE_BOLD * 16] =		RGB15(21, 21, 31);
	pal_obj_mem[1 + COLOR_MAGENTA * 16] =		RGB15(27, 0, 27);
	pal_obj_mem[1 + COLOR_MAGENTA_BOLD * 16] =	RGB15(31, 21, 31);
	pal_obj_mem[1 + COLOR_CYAN * 16] =		RGB15(0, 27, 27);
	pal_obj_mem[1 + COLOR_CYAN_BOLD * 16] =		RGB15(21, 31, 31);
	pal_obj_mem[1 + COLOR_WHITE * 16] =		RGB15(27, 27, 27);
	pal_obj_mem[1 + COLOR_WHITE_BOLD * 16] =	RGB15(31, 31, 31);
}

void loadbgtile(int pos, u16 *tile, int cidx)
{
	int i;
	u16 tmptile[CHARTILELEN];
	/* u16 color = (1 << 12) | (1 << 8) | (1 << 4) | 1; */

	for (i = 0; i < 8 * 8; i ++) {
		tmptile[i] = ((tile[i] & 0xF) ? cidx : 0) |
			((((tile[i] & 0xFF) >> 4) ? cidx : 0) << 4) |
			((((tile[i] & 0xFFF) >> 8) ? cidx : 0) << 8) |
			((tile[i] >> 12 ? cidx : 0) << 12);
		/* tmptile[i] = tile[i] & color; */
	}
	memcpy(&tile_mem[0][pos * 4], tmptile, CHARTILELEN);

}

/* 0 = transparent, 1 = color, 2 = black */
void loadsprite(int pos, u16 *tile)
{
	int i;
	u16 tmptile[CHARTILELEN];

	for (i = 0; i < 8 * 8; i ++) {
		tmptile[i] = ((tile[i] & 0xF) ? 1 : 2) |
			((((tile[i] & 0xFF) >> 4) ? 1 : 2) << 4) |
			((((tile[i] & 0xFFF) >> 8) ? 1 : 2) << 8) |
			((tile[i] >> 12 ? 1 : 2) << 12);
	}
	memcpy(&tile_mem[4][pos * 4], tmptile, CHARTILELEN);

}

void v_draw_bg(struct Map *map, struct VEngine *veng, int dudex, int dudey)
{
	int x, y;
	int sx, sy;
	int xpad = 0, ypad = 0;

	unsigned char mentr;

	char msg[200];

	for (y = 0; y < 16; y ++) {
		for (x = 0; x < 32; x ++) {
			v_putbgtile(x, y, 255);
		}
	}

	sx = dudex - 7;
	sy = dudey - 4;

	if (sx < 0) {
		xpad = (-sx);
		sx = 0;
	}

	if (sy < 0) {
		ypad = (-sy);
		sy = 0;
	}

	for (y = 0; y < 10; y ++) {
		for (x = 0; x < 15; x ++) {
			if (x < xpad || y < ypad) {
				v_putbgtile(veng->mapofsx + x,
					veng->mapofsy + y, 0);
			} else {
				mentr = map->mapdata[(sx + x - xpad) +
					((sy + y - ypad) * map->width)];
				sprintf(msg, "%dx%d (%d) = [%d]\r\n",
					(sx + x - xpad), (sy + y - ypad),
					(sx + x - xpad) + ((sy + y - ypad) * map->width),
					mentr);
				// print(msg);
				v_putbgtile(veng->mapofsx + x,
					veng->mapofsy + y, mentr);
			}
		}
	}
}

void v_draw_more_bg(
	struct Map *map,
	int fromx, int fromy,
	int cols, int lines,
	int mapx, int mapy)
{
	int x, y;

	unsigned char mentr;

	if (fromx < 0) fromx = 32 + fromx;
	if (fromy < 0) fromy = 16 + fromy;

	for (y = 0; y < lines; y ++) {
		for (x = 0; x < cols; x ++) {
			if ((mapx + x) < 0 || (mapy + y) < 0) {
				mentr = 0;
			} else if ((mapx + x) >= map->width ||
					(mapy + y) >= map->height) {
				mentr = 0;
			} else {
				mentr = map->mapdata[(mapx + x) +
					(mapy + y) * map->width];
			}
			v_putbgtile((fromx + x), (fromy + y), mentr);
		}
	}

}

void v_putbgtile(int x, int y, int type)
{
	int rx, ry;
	
	rx = (x % 32) * 2;
	ry = (y % 16) * 2;

	(&se_mem[28][0])[se_index(rx, ry, 64)] = 4 * type;
	(&se_mem[28][0])[se_index(rx + 1, ry, 64)] = 4 * type + 1;
	(&se_mem[28][0])[se_index(rx, ry + 1, 64)] = 4 * type + 2;
	(&se_mem[28][0])[se_index(rx + 1, ry + 1, 64)] = 4 * type + 3;
}

int v_spr_create(struct VEngine *veng, u16 *tile, u16 color)
{
	int t, s, i;

	i = 0;
	for (i = 0; i < 128; i ++) {
		if (veng->sprites[i].allocated == 0) {
			veng->sprites[i].allocated = 1;
			break;
		}
	}

	t = v_tile_alloc(veng);
	loadsprite(t, tile);

	s = v_spr_alloc(veng);
	obj_set_attr(&veng->obj_buffer[s],
		ATTR0_SQUARE | ATTR0_4BPP,
		ATTR1_SIZE_16,
		ATTR2_PALBANK(color) | (t * 4));

	debug("We have created a sprite (%d) [t: %d][s: %d]", i, t, s);

	return i;
}

void v_init_monster(struct VEngine *veng, u8 idx)
{
	veng->monsters[idx].mobposx = -1;
	veng->monsters[idx].mobposy = -1;
	veng->monsters[idx].active = 1;
	debug("Loading monster %d with stuff", idx);
	veng->monsters[idx].sprite =
		v_spr_create(veng, CHARTILE(33), COLOR_RED);
}

void v_init(struct VEngine *veng)
{
	int i;

	init_palette();

	for (i = 0; i < 32; i ++) {
		loadbgtile(i + 30, CHARTILE(i), COLOR_WHITE);
	}

	loadbgtile(WORLD_WALL, CHARTILE(55), COLOR_BLACK_BOLD);
	loadbgtile(WORLD_FLOOR, CHARTILE(69), COLOR_WHITE);
	loadbgtile(WORLD_DOOR, CHARTILE(58), COLOR_YELLOW);
	loadbgtile(254, CHARTILE(45), COLOR_MAGENTA);
	loadbgtile(255, CHARTILE(62), COLOR_CYAN);

	oam_init(veng->obj_buffer, 128);
	for (i = 0; i < 128; i ++) {
		veng->sprites[i].allocated = 0;
		veng->sprites_allocated[i] = 0;
		veng->tiles_allocated[i] = 0;
	}

	for (i = 0; i < 10; i ++) {
		veng->monsters[i].active = 0;
	}

	/* veng->spr_dude = &veng->obj_buffer[0]; */
	veng->spr_dude = v_spr_create(veng, CHARTILE(63), COLOR_WHITE_BOLD);

	/*
	veng->goblin = &veng->obj_buffer[1];
	obj_set_attr(veng->goblin,
		ATTR0_SQUARE | ATTR0_4BPP,
		ATTR1_SIZE_16,
		ATTR2_PALBANK(COLOR_RED_BOLD) | (4 * 1));
	*/

	REG_BG0CNT= BG_CBB(0) | BG_SBB(28) | BG_4BPP | BG_REG_64x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 |
		DCNT_OBJ | DCNT_OBJ_1D;

	veng->dudeposx = 0;
	veng->dudeposy = 0;

	veng->mapofsx = 0;
	veng->mapofsy = 14;

}

/* Reserve a tile for use by sprite */
u8 v_tile_alloc(struct VEngine *veng)
{
	u8 i;

	for (i = 0; i < 128; i ++) {
		if (veng->tiles_allocated[i] == 0) {
			veng->tiles_allocated[i] = 1;
			return i;
		}
	}
	/* Out of tiles, what do? */
	return 0;
}

/* Free a reserved tile (for use by sprites) */
void v_tile_free(struct VEngine *veng, u8 idx)
{
	veng->tiles_allocated[idx] = 0;
}

/* Reserve a sprite for use */
u8 v_spr_alloc(struct VEngine *veng)
{
	u8 i;

	for (i = 0; i < 128; i ++) {
		if (veng->sprites_allocated[i] == 0) {
			veng->sprites_allocated[i] = 1;
			return i;
		}
	}
	/* Out of sprites, what do? */
	return 0;
}

/* Free a reserved sprite */
void v_spr_free(struct VEngine *veng, u8 idx)
{
	veng->sprites_allocated[idx] = 0;
}

void v_shake_dude(struct VEngine *veng, u8 x, u8 y)
{
	int i;

	for (i = -5; i != 5; i ++) {
		obj_set_pos(SPR_DUDE,
			(16 * 7) + (veng->dudeposx * 16) + (x * i),
			(16 * 4) + (veng->dudeposy * 16) + (y * i));
		oam_copy(oam_mem, veng->obj_buffer, 11); 
		vid_vsync();
	}

}

void v_draw_dude(struct VEngine *veng)
{
	obj_set_pos(SPR_DUDE,
		(16 * 7) + (veng->dudeposx * 16),
		(16 * 4) + (veng->dudeposy * 16));
}

void v_draw_monster(struct VEngine *veng, struct World *world, u8 idx)
{
	struct Monster *mob = &world->monsters[idx];
	struct VEngine_Monster *vmob = &veng->monsters[idx];

	/* debug("Would like to draw monster %d", idx); */
	vmob->mobposx = veng->dudeposx + (mob->x - world->map->dudex);
	vmob->mobposy = veng->dudeposy + (mob->y - world->map->dudey);

	if (!mob->alive || mob->x < veng->mapofsx ||
			mob->x > veng->mapofsx + 15 ||
			mob->y < veng->mapofsy - 2 ||
			mob->y > veng->mapofsy + 10) {
		/* draw off-screen if not visible */
		/* debug("mob %d is drawn off-screen", idx); */
		obj_set_pos(SPR_MOB(idx), -16, -16);
	} else {
		/* debug("mob %d is drawn on-screen", idx); */
		obj_set_pos(SPR_MOB(idx),
			(16 * 7) + (vmob->mobposx * 16),
			(16 * 4) + (vmob->mobposy * 16));
	}

}

/* Animate dude's movements */

void v_move_left(struct VEngine *veng)
{
	int i;

	for (i = 0; i < 16; i += 4) {
		obj_set_pos(SPR_DUDE,
			(16 * 7) + (veng->dudeposx * 16) - i,
			(16 * 4) + (veng->dudeposy * 16));
		oam_copy(oam_mem, veng->obj_buffer, 1); 
		vid_vsync();
	}
	veng->dudeposx --;
}

void v_move_right(struct VEngine *veng)
{
	int i;

	for (i = 0; i < 16; i += 4) {
		obj_set_pos(SPR_DUDE,
			(16 * 7) + (veng->dudeposx * 16) + i,
			(16 * 4) + (veng->dudeposy * 16));
		oam_copy(oam_mem, veng->obj_buffer, 1); 
		vid_vsync();
	}
	veng->dudeposx ++;
}

void v_move_up(struct VEngine *veng)
{
	int i;

	for (i = 0; i < 16; i += 4) {
		obj_set_pos(SPR_DUDE,
			(16 * 7) + (veng->dudeposx * 16),
			(16 * 4) + (veng->dudeposy * 16) - i);
		oam_copy(oam_mem, veng->obj_buffer, 1); 
		vid_vsync();
	}
	veng->dudeposy --;
}

void v_move_down(struct VEngine *veng)
{
	int i;

	for (i = 0; i < 16; i += 4) {
		obj_set_pos(SPR_DUDE,
			(16 * 7) + (veng->dudeposx * 16),
			(16 * 4) + (veng->dudeposy * 16) + i);
		oam_copy(oam_mem, veng->obj_buffer, 1); 
		vid_vsync();
	}
	veng->dudeposy ++;
}

/* Scroll the dude & monsters */
void v_scroll_sprites(struct VEngine *veng, struct World *world, int x, int y)
{
	int i, m, steps = 0;
	struct Monster *mob;

	if (y == -1) steps = 5;
	else if (y == 1) steps = 4;
	else if (x == 1 || x == -1) steps = 7;
	else debug("Fail");

	for (i = 0; i < 16 * steps; i += 5) {
		obj_set_pos(SPR_DUDE,
			(16 * 7) + (veng->dudeposx * 16) + (x * i),
			(16 * 4) + (veng->dudeposy * 16) + (y * i));

		for (m = 0; m < world->monstercount; m ++) {
			mob = &world->monsters[m];
			if (!mob->alive || mob->x < veng->mapofsx ||
					mob->x > veng->mapofsx + 15 ||
					mob->y < veng->mapofsy - 2 ||
					mob->y > veng->mapofsy + 10) {
				continue;
			}
			obj_set_pos(SPR_MOB(m),
				(16 * 7) + (veng->monsters[m].mobposx * 16) +
				(x * i),
				(16 * 4) + (veng->monsters[m].mobposy * 16) +
				(y * i));
		}
		oam_copy(oam_mem, veng->obj_buffer, 1 + world->monstercount); 

		REG_BG0HOFS = (veng->mapofsx * 16) + -(x * i);
		REG_BG0VOFS = (veng->mapofsy * 16) + -(y * i);
		vid_vsync();
	}

	veng->mapofsy += (-y * steps);
	veng->mapofsx += (-x * steps);

	if (y != 0) veng->dudeposy = 0;
	if (x != 0) veng->dudeposx = 0;
}

void v_scroll_at_edge(struct VEngine *veng, struct World *world)
{
	/* At the bottom edge */
	if (veng->dudeposy == 5) {
		v_draw_more_bg(world->map,
			veng->mapofsx, veng->mapofsy + 10, 15, 5,
			world->map->dudex - 7 - veng->dudeposx,
			world->map->dudey + 6 - veng->dudeposy);
		v_scroll_sprites(veng, world, 0, -1);
	}

	/* At the top edge */
	if (veng->dudeposy == -4) {
		v_draw_more_bg(world->map,
			veng->mapofsx, veng->mapofsy - 4, 15, 4,
			world->map->dudex - 7 - veng->dudeposx,
			world->map->dudey - 8 - veng->dudeposy);
		v_scroll_sprites(veng, world, 0, 1);
	}

	/* At the right edge */
	if (veng->dudeposx == 7) {
		v_draw_more_bg(world->map,
			veng->mapofsx + 15, veng->mapofsy, 8, 10,
			world->map->dudex + 8 - veng->dudeposx,
			world->map->dudey - 4 - veng->dudeposy);
		v_scroll_sprites(veng, world, -1, 0);
	}

	/* At the left edge */
	if (veng->dudeposx == -7) {
		v_draw_more_bg(world->map,
			veng->mapofsx - 9, veng->mapofsy, 9, 10,
			world->map->dudex - 16 - veng->dudeposx,
			world->map->dudey - 4 - veng->dudeposy);
		v_scroll_sprites(veng, world, 1, 0);
	}

	REG_BG0HOFS = veng->mapofsx * 16;
	REG_BG0VOFS = veng->mapofsy * 16;
}

/* vim: set sw=8 noet: */
