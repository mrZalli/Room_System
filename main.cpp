#include "room.hh"
#include "entity_manager.hh"
#include "ascii_drawer.hh"
//#include <iostream>
#include <ncurses.h>
#include <string>

//using namespace std;
using namespace Room_System;
using namespace Coordinates;
//using namespace ascii_draw;

void init_ncurses() {
	initscr();
	raw();//cbreak();
	keypad(stdscr, true);
	noecho();
	curs_set(0); // invisible cursor

	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_WHITE, COLOR_BLUE);
		attron(COLOR_PAIR(1));
	}
	else {
		printw("This terminal doesn't support colors\n");
	}
}

int main()
{
	Entity_manager ent_man{};
	
	Room* r1 = new Room{{9,4}, "the first room"};
	Room* r2 = new Room{{7,4}, "the second room"};
	Room* r3 = new Room{{3,9}, "the magic room", room_type::corridor};
	Room* r4 = new Room{{32,13}, "the big room", room_type::large_room};
	
	Room::room_tr rtr1to2 = Room::room_tr(r2, pos_t(8,3), area_t({0,2}));
	r1->add_bi_room_tr(rtr1to2);
	
	Room::room_tr rtr1to3 = Room::room_tr(r3, pos_t(2,10), area_t({6,0}));
	r1->add_bi_room_tr(rtr1to3);
	
	Room::room_tr rtr2to3 = Room::room_tr(r3, pos_t(2,0), area_t({3,5}));
	r2->add_bi_room_tr(rtr2to3);
	
	Room::room_tr rtr1to4 = Room::room_tr(r4, pos_t(0,10), area_t({10,3}));
	r1->add_bi_room_tr(rtr1to4);
	
	Room::room_tr rtr4to4 = Room::room_tr(r4, pos_t(2,14), area_t({13,0}));
	r4->add_bi_room_tr(rtr4to4);
	
	// the ownership is given to the entity
	Comps::Movable_position* pc_pos = new Comps::Movable_position(r1, pos_t{2,2});
	Comps::Name* pc_name = new Comps::Name("Albert A Asimov");
	Entity* player_ptr = ent_man.add_entity_from_archetype(std::string("player"), {pc_pos, pc_name});

	init_ncurses();

	dim_t scrdim;
	getmaxyx(stdscr, scrdim.l, scrdim.w);
	int ch = 0;

	Ascii_drawer adr(nullptr, stdscr, player_ptr, '@');
	
	while(true) { // main loop
		adr.add_debug_message(player_ptr->get_component_with_name("position").get_value_str());
		
		clear();

		adr.draw_vis_array();
		adr.print_debug_messages();
		
		refresh();

		bool move_key_pressed = false;
		pos_t displ{};
		
		ch = wgetch(stdscr);
		adr.add_debug_message("key: " + std::to_string(ch));
		
		// stop program on shift + q
		if (ch == 81) {
			break;
		}
		
		// update window size
		else if (ch == -1) { 
			getmaxyx(stdscr, scrdim.l, scrdim.w);
			ch = 0;
		}
		
		//movement
		else if (ch == KEY_LEFT) {
			move_key_pressed = true;
			displ = pos_t(-1, 0);
		}
		else if (ch == KEY_RIGHT) {
			move_key_pressed = true;
			displ = pos_t( 1, 0);
		}
		else if (ch == KEY_UP) {
			move_key_pressed = true;
			displ = pos_t( 0,-1);
		}
		else if (ch == KEY_DOWN) {
			move_key_pressed = true;
			displ = pos_t( 0, 1);
		}
		/*//only numpad keys. don't work for some reason
		else if (ch == KEY_A1 || ch == 55) {
			move_key_pressed = true;
			displ = pos_t(-1,-1);
		} else if (ch == KEY_A3 || ch == 57) {
			move_key_pressed = true;
			displ = pos_t(1,-1);
		} else if (ch == KEY_C1 || ch == 49) {
			move_key_pressed = true;
			displ = pos_t(-1,1);
		} else if (ch == KEY_C3 || ch == 51) {
			move_key_pressed = true;
			displ = pos_t(1,1);
		}
		else if (ch == KEY_B2) { // numpad center
		   move_key_pressed = true;
		   displ = pos_t(0,0);
		}*/
		
		// if we didn't move displ is (0,0)
		pc_pos->move_pos(displ);
		
		if (ch == 32) { // space
			adr.update(true);
		}
		else if (move_key_pressed) {
			adr.update();
		}
	}

	endwin();
	
	delete r1;
	delete r2;
	delete r3;
	delete r4;

	return 0;
}


