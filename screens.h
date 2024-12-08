#include <iostream>
#include <string.h>
#include <curses.h>

#ifndef SCREENS_H
#define SCREENS_H

class Screen {
    public:
	std::string name;
	int height;
	int width;
	int pos_y;
	int pos_x;
	WINDOW *screen;

    Screen(std::string name, int height, int width, int pos_y, int pos_x) {
	this->name = name;
	this->height = height;
	this->width = width;
	this->pos_y = pos_y;
	this->pos_x = pos_x;
	this->screen = newwin(height, width, pos_y, pos_x);
    }
};

#endif
