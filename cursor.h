#include <iostream>
#include <curses.h>

#ifndef CURSOR_H
#define CURSOR_H

// Fairly simple cursor class to give the player a visual representation of their selections.

class Cursor {
    public:
	int pos_x;
	int pos_y;
	int size;
	int width;
	// This enumerator simply help me to keep track of which direction the player is moving in
	enum Directions {left, right, up, down};

	void draw(WINDOW *screen) {
	    // Activate the cursor color
	    wattron(screen, COLOR_PAIR(2));
	    // A nested for loop to put boxes only around the edge of the cursor
	    for (int i = pos_y; i < pos_y + size; i++) {
		if (i == pos_y or i == pos_y + size-1) {
		    mvwprintw(screen, i, pos_x, "0");
		}
	    }
	    // Always turn off the color attributes when finished
	    wattroff(screen, COLOR_PAIR(2));
	}

	void move(int direction, WINDOW *screen) {
	    // Get the bounds of our box
	    int max_x, max_y;
	    getmaxyx(screen, max_y, max_x);
	    // Switch statement to parse the inputted direction
	    // We want the cursor to get close to the edges but never really touch the edges
	    // hence the 2's, the ones however ensure the cursor moves within the bounds of the tile
	    // borders.
	    switch (direction) {
		case left:
		    pos_x -= width + 1;
		    if (pos_x < 2) {
			pos_x += width + 1;
		    }
		    break;
		case right:
		    pos_x += width + 1;
		    if (pos_x + width > max_x-2) {
			pos_x -= width + 1;
		    }
		    break;
		case up:
		    pos_y -= size + 1;
		    if (pos_y < 2) {
			pos_y += size + 1;
		    }
		    break;
		case down:
		    pos_y += size + 1;
		    if (pos_y + size > max_y-2) {
			pos_y -= size + 1;
		    }
		    break;
	    }
	}

	// Resets the cursors position whenever the user switches to another tab
	void next_tab() {
	    pos_y = 2;
	    pos_x = 2;
	}

    Cursor(int pos_x, int pos_y, int size) {
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->size = size;
	this->width = size + size / 2;
    }
};

#endif
