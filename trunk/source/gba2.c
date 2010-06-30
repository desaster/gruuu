#include <string.h>
#include <tonc.h>
#include <stdlib.h>

#include "util.h"

#include "map.h"
#include "genmap.h"
#include "rengine.h"
#include "vengine.h"
#include "joy.h"

#define HTILES 15
#define VTILES 10

void init_world(struct World *world, struct VEngine *veng)
{
	int i, m;

	/* FIXME: why big maps appear broken? */
	genmap(world->map, 40, 40);

	world->monstercount = 0;
	for (i = 0; i < 15; i ++) {
		m = world->monstercount ++;
		init_monster(world, m);
		v_init_monster(veng, m);
	}
}

int main()
{
	int i;
	struct VEngine veng;
	struct World world;
	JoyState* joy;

	v_init(&veng);
	joy_init();

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

	init_world(&world, &veng);

	/* obj_set_pos(dude, (16 * 7), (16 * 4)); */
	
	v_draw_bg(world.map, &veng, world.map->dudex, world.map->dudey);

	while(1)
	{
		vid_vsync();
		joy = joy_poll();
		key_poll();

		if (key_hit(KEY_A)) {
			/*
			v_draw_more_bg(world.map, veng.mapofsx, veng.mapofsy - 4, 15, 4,
				world.map->dudex - 6 - veng.dudeposx,
				world.map->dudey - 4 - 4 - veng.dudeposy);
				*/
		}

		if (key_hit(KEY_START)) {
			v_init(&veng);
			init_world(&world, &veng);

			/*
			obj_set_pos(veng.obj_buffer[veng.spr_dude], (16 * 7), (16 * 4));
			*/

			v_draw_bg(world.map, &veng,
				world.map->dudex, world.map->dudey);
		}

		/*
		if (key_hit(KEY_LEFT)) {
			switch (r_move_left(&world)) {
				case 1:
					v_move_left(&veng);
					break;
				case 2:
					v_shake_dude(&veng, 1, 0);
					break;
			}
		} else if (key_hit(KEY_RIGHT)) {
			switch (r_move_right(&world)) {
				case 1:
					v_move_right(&veng);
					break;
				case 2:
					v_shake_dude(&veng, 1, 0);
					break;
			}
		} else if (key_hit(KEY_UP)) {
			switch (r_move_up(&world)) {
				case 1:
					v_move_up(&veng);
					break;
				case 2:
					v_shake_dude(&veng, 0, 1);
					break;
			}
		} else if (key_hit(KEY_DOWN)) {
			switch (r_move_down(&world)) {
				case 1:
					v_move_down(&veng);
					break;
				case 2:
					v_shake_dude(&veng, 0, 1);
					break;
			}
		}
		*/

		if (joy->moving) {
			switch (r_move(&world, joy->dx, joy->dy)) {
				case 1:
					v_move(&veng, joy->dx, joy->dy);
					break;
				case 2:
					v_shake_dude(&veng, 0, 1);
					break;
			}
			joy_handled();
		}
		v_move_co(&veng);

		v_scroll_at_edge(&veng, &world);

		v_draw_dude(&veng);

		for (i = 0; i < world.monstercount; i ++) {
			v_draw_monster(&veng, &world, i);
		}

		oam_copy(oam_mem, veng.obj_buffer, 1 + world.monstercount);
	}

	return 0;
}

/* vim: set sw=8 noet: */
