#include <string>
#include <curses.h>
#include <chrono>

#ifndef BULLET_H
#define BULLET_H

struct Bullet {
    int index;
    int damage;
    int type;
    long spawn_time;
    int posx;
    int pos[2] = {0};

    void update(long time) {
	if ((time + spawn_time) % 50 == 0 && (time + spawn_time) % 50 == 0) {
	    pos[1] += 1;
	}
    }

    void print(WINDOW *screen) {
	if (type == 1) mvwprintw(screen, pos[0], pos[1], "o");
	if (type == 2) mvwprintw(screen, pos[0], pos[1], "@");
	if (type == 3) mvwprintw(screen, pos[0], pos[1], ">>");
    }

    Bullet(int type, int y, int x, int index, long time) {
	this->pos[0] += y;
	this->pos[1] += x;
	spawn_time = time;
	this->type = type;
	this->index = index;
    }

};

#endif
