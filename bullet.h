#include <string>
#include <curses.h>
#include <chrono>

#ifndef BULLET_H
#define BULLET_H

struct Bullet {
    int index;
    int damage;
    int type;
    long last_time;
    long spawn_time;
    int posx;
    int pos[2] = {0};

    void update(long time) {
	if (time >= last_time + 25) {
	    last_time = time;
	    pos[1] += 1;
	}
    }

    void print(WINDOW *screen) {
	if (type == 0) mvwprintw(screen, pos[0], pos[1], "o");
	if (type == 1) mvwprintw(screen, pos[0], pos[1], "@");
	if (type == 2) mvwprintw(screen, pos[0], pos[1], ">>");
	if (type == 4) mvwprintw(screen, pos[0], pos[1], "+$50");
    }

    Bullet(int type, int y, int x, int index, long time) {
	this->pos[0] += y;
	this->pos[1] += x;
	last_time = time;
	spawn_time = time;
	this->type = type;
	this->index = index;
    }

};

#endif
