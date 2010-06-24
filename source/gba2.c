#include <string.h>
#include <tonc.h>
#include <stdlib.h>

#include "util.h"

#include "map.h"
#include "genmap.h"
#include "rengine.h"
#include "vengine.h"

#define HTILES 15
#define VTILES 10

void init_world(struct World *world)
{
	int i;

	/* FIXME: why big maps appear broken? */
	genmap(world->map, 40, 40);

	for (i = 0; i < 1; i ++) {
		init_monster(world, i);
	}
	world->monstercount = 1;
}

int main()
{
	int i;
	struct VEngine veng;
	struct World world;

	v_init(&veng);

	/* for a random seed, wait for user input */
	i = 0;
	while (1) {
		vid_vsync();
		key_poll();
		v_putbgtile(5, 4, 37);
		v_putbgtile(6, 4, 48);
		v_putbgtile(7, 4, 51);
		v_putbgtile(8, 4, 51);
		v_putbgtile(9, 4, 51);
		if (key_hit(KEY_A) || key_hit(KEY_START)) {
			break;
		}
		i ++;
	}

	/* sqran(i); */
	sqran(4);

	init_world(&world);

	/* obj_set_pos(dude, (16 * 7), (16 * 4)); */
	
	v_draw_bg(world.map, &veng, world.map->dudex, world.map->dudey);

	while(1)
	{
		vid_vsync();
		key_poll();

		if (key_hit(KEY_A)) {
			v_draw_more_bg(world.map, veng.mapofsx, veng.mapofsy - 4, 15, 4,
				world.map->dudex - 6 - veng.dudeposx,
				world.map->dudey - 4 - 4 - veng.dudeposy);
		}

		if (key_hit(KEY_START)) {
			init_world(&world);
			v_init(&veng);

			/*
			obj_set_pos(veng.obj_buffer[veng.spr_dude], (16 * 7), (16 * 4));
			*/

			v_draw_bg(world.map, &veng,
				world.map->dudex, world.map->dudey);
		}

		if (key_hit(KEY_LEFT) && r_move_left(&world)) {
			v_move_left(&veng);
		} else if (key_hit(KEY_RIGHT) && r_move_right(&world)) {
			v_move_right(&veng);
		} else if (key_hit(KEY_UP) && r_move_up(&world)) {
			v_move_up(&veng);
		} else if (key_hit(KEY_DOWN) && r_move_down(&world)) {
			v_move_down(&veng);
		}

		if (v_at_edge(&veng, &world)) {
			debug("Reuna tuli vastaan!");
		}

		v_scroll_at_edge(&veng, &world);

		/* debug("dude will be drawn at [%d]",
			(16 * 7) + (veng.dudeposx * 16)); */
		v_draw_dude(&veng);

		for (i = 0; i < world.monstercount; i ++) {
			struct Monster *mob;
			mob = &world.monsters[i];
			v_draw_monster(&veng, &world, i);
			/*
			if (!(mob->x >= mapofsx &&
					mob->x <= mapofsx + 15 &&
					mob->y >= mapofsy &&
					mob->y <= mapofsy + 10)) {
				obj_hide(goblin);
				continue;
			}
			obj_unhide(goblin, 0);
			*/
			/*
			debug("*** Monster %d", i);
			debug(" mob on map [%dx%d]", mob->x, mob->y);
			debug(" dude on map [%dx%d]",
				world.map->dudex, world.map->dudey);
			*/

			/*
			mobposx = veng.dudeposx + (mob->x - world.map->dudex);
			mobposy = veng.dudeposy + (mob->y - world.map->dudey);
			obj_set_pos(veng.goblin,
				(16 * 7) + (mobposx * 16),
				(16 * 4) + (mobposy * 16));
			break;
			*/
		}

		oam_copy(oam_mem, veng.obj_buffer, 1 + world.monstercount);
	}

	return 0;
}

/* vim: set sw=8 noet: */
