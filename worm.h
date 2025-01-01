#include <curses.h>
#include <vector>
#include <chrono>
#include <string>
#include <stdbool.h>

#ifndef WORM_H
#define WORM_H

class Worm {
    int WIDTH = 14;
    int SIZE = 7;
public:
    int health;
    int speed;
    int pos[2];
    int type;
    int row;
    int attr;
    long last_time;
    long last_attack;
    bool slowed;
    std::string image[7];

    template<typename O>
    bool check_collisions(std::vector<O> *objects) {
	for (int i = 0; i < objects->size(); i++) {
	    if ((*objects)[i].collide_x >= pos[1] && (*objects)[i].pos[0] == pos[0]) {
		return true;
	    }
	}
	return false;
    }

    template<typename O, typename T>
    void move(std::vector<O> *objects, T* tiles, long time) {
	if (!check_collisions(objects)) {
	    pos[1] -= 1;
	} else {
	    for (int i = 0; i < objects->size(); i++) {
		if ((*objects)[i].collide_x >= pos[1] && (*objects)[i].pos[0] == pos[0]) {
		    if (time >= last_attack + 400) {
			last_attack = time;
			(*objects)[i].health -= 10;
			if ((*objects)[i].health <= 0) {
			    objects->erase(objects->begin() + i);
			    (*tiles)[(*objects)[i].row][(*objects)[i].col].occupied = false;
			}
		    }
		}
	    }
	}
    }

    template<typename O, typename T>
    void update(std::vector<O> *objects, T* tiles, long time) {
	if (last_time + (speed * (slowed ? 2 : 1)) <= time) {
	    last_time = time;
	    move(objects, tiles, time);
	}
    }

    void print(WINDOW *screen, int maxx) {
	int len = maxx - pos[1];
	wattron(screen, COLOR_PAIR(attr));
	for (int i = 0; i < SIZE; i++) {
	    mvwprintw(screen, pos[0] + i, pos[1], image[i].substr(0, len > 14 ? 14 : len).data()); 
	}
	wattroff(screen, COLOR_PAIR(attr));
    }
	    

    Worm(int row, int type, long time) {
	this->row = row;
	last_time = time;
	last_attack = 0;
	slowed = false;
	pos[0] = row * SIZE + row + 2;
	pos[1] = 10 * WIDTH + 10 + 2;
	attr = 0;
	if (type == 0) {
	    health = 200;
	    speed = 500;
	    image[0] = "     _     _  ";
	    image[1] = "    /__   /_  ";
	    image[2] = " *_/   \\_|  \\ ";
	    image[3] = " *_|   |_|   |";
	    image[4] = "   \\___/ |__/ ";
	    image[5] = "    \\_    \\_  ";
	    image[6] = "              ";
	}
	if (type == 1) {
	    health = 100;
	    speed = 400;
	    image[0] = "              ";
	    image[1] = "              ";
	    image[2] = "     _____    ";
	    image[3] = "  __/ ___ \\__ ";
	    image[4] = " (___/   \\___)";
	    image[5] = "              ";
	    image[6] = "              ";
	}
	if (type == 2) {
	    health = 300;
	    speed = 600;
	    image[0] = "              ";
	    image[1] = "              ";
	    image[2] = "     _______  ";
	    image[3] = "    /  \\ /  \\ ";
	    image[4] = "(**)|___|___| ";
	    image[5] = "      | | |   ";
	    image[6] = "              ";
	}
	if (type == 3) {
	    health = 150;
	    speed = 200;
	    image[0] = "   ___ ___  _ ";
	    image[1] = "  (*__|_  \\/ \\";
	    image[2] = " /      \\  \\_/";
	    image[3] = "/       _\\ |   ";
	    image[4] = "       / | |  ";
	    image[5] = "         \\ /  ";
	    image[6] = "          V   ";
	}
    }

};

#endif
