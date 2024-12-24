#include <curses.h>
#include <iostream>
#include "bullet.h"

#ifndef TERMS_H
#define TERMS_H

class Term {
public:
    int health;
    int cost;
    int cooldown;
    int type;
    int attr;
    int bullet;
    int bullet_pos[2];
    int pos[2];
    int size = 7;
    long shoot_time = 0;
    std::string name;
    std::string image[7]; 

    void print(WINDOW *screen) {
	wattron(screen, COLOR_PAIR(attr));
	for (int i = 0; i < size; i++) {
	    mvwprintw(screen, pos[0] + i, pos[1], image[i].data()); 
	}
	wattroff(screen, COLOR_PAIR(attr));
    }


    Term(int type, int row, int col, long time) {
	pos[0] = row * size + row + 2;
	pos[1] = col * size * 2 + col + 2;
	this->type = type;
	shoot_time += time;
	health = 10;

	if (type == 0) {
	    image[0] = "     _____    ";
	    image[1] = "    /     \\   ";
	    image[2] = "   |          ";
	    image[3] = "   |   ()     ";
	    image[4] = "   |          ";
	    image[5] = "    \\_____/   ";
	    image[6] = "              ";
	    attr = 0;
	    bullet = 0;
	    bullet_pos[0] = pos[0] + 3;
	    bullet_pos[1] = pos[1] + 9;
	    cost = 100;
	    cooldown = 4000;
	    name = "C-Shooter";
	}
	if (type == 1) {
	    image[0] = "     _____    ";
	    image[1] = "    /     \\   ";
	    image[2] = "   |          ";
	    image[3] = "   |  [+][+]  ";
	    image[4] = "   |          ";
	    image[5] = "    \\_____/   ";
	    image[6] = "              ";
	    attr = 0;
	    bullet = 0;
	    bullet_pos[0] = pos[0] + 3;
	    bullet_pos[1] = pos[1] + 10;
	    cost = 200;
	    cooldown = 4000;
	    name = "C-Squared";
	}
	if (type == 2) {
	    image[0] = "    ______    ";
	    image[1] = "  _|*     |_  ";
	    image[2] = " |  ---   | | ";
	    image[3] = " |  _____/  | ";
	    image[4] = " |_|   ___ _| ";
	    image[5] = "   |     *|   ";
	    image[6] = "    ------    ";
	    attr = 3;
	    bullet = 1;
	    bullet_pos[0] = pos[0] + 3;
	    bullet_pos[1] = pos[1] + 7;
	    cost = 150;
	    cooldown = 4000;
	    name = "P-ice-thon";
	}
	if (type == 3) {
	    image[0] = "              ";
	    image[1] = "              ";
	    image[2] = "    #^^^^#    ";
	    image[3] = "  /\\|*__*|/\\  ";
	    image[4] = "   /\\____/\\   ";
	    image[5] = "   (<    >)   ";
	    image[6] = "              ";
	    attr = 1;
	    bullet = 2;
	    bullet_pos[0] = pos[0] + 3;
	    bullet_pos[1] = pos[1] + 11;
	    cost = 175;
	    cooldown = 4000;
	    name = "Tetanis Rust";
	}	
	if (type == 4) {
	    image[0] = " __           ";
	    image[1] = "|__|          ";
	    image[2] = "|    | |      ";
	    image[3] = "     |-|      ";
	    image[4] = "     | |  __  ";
	    image[5] = "         |__| ";
	    image[6] = "         |    ";
	    attr = 3;
	    bullet = 4;
	    bullet_pos[0] = pos[0] + 2;
	    bullet_pos[1] = pos[1] + 6;
	    cost = 50;
	    cooldown = 15000;
	    name = "Pay-HP";
	}

    };
};

#endif
