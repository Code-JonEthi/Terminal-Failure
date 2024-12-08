#include <iostream>
#include <string.h>
#include <curses.h>

#ifndef SCREENS_H
#define SCREENS_H

// Screen is a very simple class to help keep track of my different windows taskes with different actions

class Screen {
    public:
	std::string name;
	int height;
	int width;
	int pos_y;
	int pos_x;
	WINDOW *screen;

	// Each screen should have a border around it with its name written in the top left corner
	void draw() {
	    wattron(screen, COLOR_PAIR(1));
	    box(screen, 0, 0);
	    mvwprintw(screen, 0, 2, name.c_str());
	    wattroff(screen, COLOR_PAIR(1));
	}

    Screen(std::string name, int height, int width, int pos_y, int pos_x) {
	this->name = name;
	this->height = height;
	this->width = width;
	this->pos_y = pos_y;
	this->pos_x = pos_x;
	this->screen = subwin(stdscr, height, width, pos_y, pos_x);
    }
};

#endif
