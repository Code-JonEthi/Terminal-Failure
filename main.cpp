#include <iostream>
#include <chrono>
#include <vector>
#include <curses.h>
#include <stdbool.h>
#include <thread>
#include "cursor.h"
#include "terms.h"

using namespace std::chrono;
using namespace std::chrono_literals;

struct Tile {
    int pos[2];
    int size;
    int width;
    bool occupied;
    
    void print(WINDOW *screen);
};

void Tile::print(WINDOW *screen) {
    for (int i = -1; i < size + 1; i ++) {
	if (i == -1 or i == size) {
	    mvwprintw(screen, pos[0] + i + 1, pos[1], "+--------------+");
	} else {
	    mvwprintw(screen, pos[0] + i + 1, pos[1], "|              |");
	}
    }
}

int main() {
    std::this_thread::sleep_for(0.6ms);
    initscr();
    cbreak();
    start_color();
    noecho();
    keypad(stdscr, true);
    timeout(1);
    curs_set(FALSE);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);

    int maxes[2];
    getmaxyx(stdscr, maxes[0], maxes[1]);

    // Init windows
    enum Pages {menu, game, tips};
    WINDOW *screens[2];
    screens[menu] = newwin(maxes[0]/4, maxes[1], 0, 0);
    screens[game] = newwin(maxes[0] - maxes[0]/4, maxes[1], maxes[0]/4, 0);

    const int TILE_SIZE = 7;
    const int WIDTH = TILE_SIZE * 2;
    int gamemaxes[2];
    
    getmaxyx(screens[game], gamemaxes[0], gamemaxes[1]);
    Tile tiles[gamemaxes[0] / TILE_SIZE][gamemaxes[1] / WIDTH];

    // Init timer
    auto t1 = high_resolution_clock::now();
    auto time = duration_cast<milliseconds>(t1.time_since_epoch());
    
    // Init tiles
    const int ROWS = 4;
    const int COLS = 12;
    for (int i = 0; i < ROWS; i++) {
	for (int j = 0; j < COLS; j++) {
	    Tile t;
	    t.pos[0] = i * TILE_SIZE + i + 1;
	    t.pos[1] = j * WIDTH + j + 1;

	    t.size = TILE_SIZE;
	    t.width = WIDTH;
	    t.occupied = false;
	    t.print(screens[game]);
	    tiles[i][j] = t;
	}
    }

    // Init Terms
    int type = 1;
    std::vector<Term> terms;
    std::vector<Term> termplates;

    // Init bullet vector
    std::vector<Bullet> bullets;

    // Init cursor
    Cursor cursor(1, 1, TILE_SIZE); 

    int key;
    int screendex;
    screendex = game;
    bool running = true;
    while (running) {
	// Get elapsed time
	t1 = high_resolution_clock::now();
	time = duration_cast<milliseconds>(t1.time_since_epoch());

	// Input handling
	key = getch();
	if (key == 'q') {
	    running = false;
	}
	if (key == 'h') cursor.move(Cursor::left, ROWS, COLS);
	if (key == 'j') cursor.move(Cursor::down, ROWS, COLS);
	if (key == 'k') cursor.move(Cursor::up, ROWS, COLS);
	if (key == 'l') cursor.move(Cursor::right, ROWS, COLS);
	if (key == 't') {
	    if (!tiles[cursor.pos[0]][cursor.pos[1]].occupied) {
		Term t(type, cursor.pos[0], cursor.pos[1], terms.size(), time.count());
		tiles[cursor.pos[0]][cursor.pos[1]].occupied = true;
		terms.push_back(t);
	    }
	}
	if (key == '1') type = 1;
	if (key == '2') type = 2;
	if (key == '3') type = 3;
	if (key == '4') type = 4;

	// Render each page
	for (WINDOW *screen :screens) {
	    werase(screen);
	    box(screen, 0, 0);
	}
	mvwprintw(screens[menu], 0, 2, "Menu");
	mvwprintw(screens[game], 0, 2, "Game");

	// Render the background
	for (int i = 0; i < ROWS; i++) {
	    for (int j = 0; j < COLS; j++) {
		tiles[i][j].print(screens[game]);
	    }
	}

	// Update and Render the Terms
	for (auto term : termplates) {
	    term.print(screens[menu]);
	}
	for (int i = 0; i < terms.size(); i++) {
	    // "Kill" the term
	    if (terms[i].health <= 0) terms.erase(terms.begin() + i);

	    // Shoot bullets
	    if (time.count() >= terms[i].shoot_time + 4000) {
		terms[i].shoot_time = time.count();
		Bullet b(terms[i].bullet, terms[i].bullet_pos[0], terms[i].bullet_pos[1], bullets.size(), time.count());
		bullets.push_back(b);
	    }

	    // Render term
	    terms[i].print(screens[game]);
	}

	// Update and render the bullets
	for (int i = 0; i < bullets.size(); i++) {
	    // "Kill" bullet
	    //if (bullet.pos[1] >= gamemaxes[1] or time - bullet.spawn_time >= 10000) bullets.erase(bullets.begin() + bullet.index);

	    // Update the bullet
	    bullets[i].update(time.count());

	    // Render the bullet
	    bullets[i].print(screens[game]);
	}

	// Update and render the cursor
	wattron(screens[screendex], COLOR_PAIR(cursor.attr));
	cursor.print(screens[screendex]);
	wattroff(screens[screendex], COLOR_PAIR(cursor.attr));

	for (WINDOW *screen :screens) {
	    wnoutrefresh(screen);
	}
	doupdate();
    }

    for (WINDOW *screen : screens) {
	delwin(screen);	
    }
    endwin();
	
    return 0;
}
