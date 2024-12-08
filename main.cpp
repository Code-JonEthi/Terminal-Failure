#include <stdio.h>
#include <vector>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include "screens.h"

using namespace std;
int main() {
    initscr();
    start_color();
    cbreak();
    noecho();
    timeout(50);
    curs_set(0);
    char key;

    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    vector<Screen> screens;
    screens.push_back(Screen("Menu", maxy/4, maxx, 0, 0));
    screens.push_back(Screen("Game", maxy/2 + maxy/6, maxx, maxy/4-1, 0));
    screens.push_back(Screen("Tooltips", maxy/6, maxx, maxy-maxy/6, 0));

    int screen;

    for (Screen item : screens) {
	box(item.screen, 0, 0);
    }

    screen = 0;
    bool running = true;
    while(running) {
	key = wgetch(screens.at(screen).screen);
	switch (key) {
	    case '\t':
		mvwprintw(screens.at(screen).screen, 0, 2, screens.at(screen).name.c_str());
		screen = screen < 2 ? screen < 1 ? 1 : 2 : 0;
		break;
	    case 'q':
		running = false;
		break;
	    default: continue;
	}
    }

    endwin();
    return 0;
    }
