#include <iostream>
#include <curses.h>
#ifndef CURSOR_H
#define CURSOR_H

// Fairly simple cursor class to give the player a visual representation of their selections.

class Cursor {
    public:
	int pos[2];
	int size;
	int width;
	int attr;
	// This enumerator simply help me to keep track of which direction the player is moving in
	enum Directions {left, right, up, down};

	void print(WINDOW *screen) {
	    wattron(screen, COLOR_PAIR(2));
	    for (int i = 0; i < size + 2; i++) {
		if (i == 0 or i == size + 1) mvwprintw(screen, (pos[0] * size + 1 + pos[0]) + i, (pos[1] * width + 1 + pos[1]), "0000000000000000");
		else {
		    mvwprintw(screen, (pos[0] * size + 1 + pos[0]) + i, (pos[1] * width + 1 + pos[1]), "0");
		    mvwprintw(screen, (pos[0] * size + 1 + pos[0]) + i, (pos[1] * width + 1 + pos[1]) + width + 1, "0");
		}
	    }
	    wattroff(screen, COLOR_PAIR(2));
	}

	void move(int direction, int rows, int cols) {
	    switch (direction) {
		case left:
		    pos[1] -= 1;
		    if (pos[1] < 0) pos[1] += 1;
		    break;
		case right:
		    pos[1] += 1;
		    if (pos[1] >= cols) pos[1] -= 1;
		    break;
		case up:
		    pos[0] -= 1;
		    if (pos[0] < 0) pos[0] += 1;
		    break;
		case down:
		    pos[0] += 1;
		    if (pos[0] >= rows) pos[0] -= 1;
		    break;
	    }
	}

    Cursor(int pos_x, int pos_y, int size) {
	this->size = size;
	width = size * 2;
	pos[1] = pos_x;
	pos[0] = pos_y;
	attr = 2;
    }
};

#endif
