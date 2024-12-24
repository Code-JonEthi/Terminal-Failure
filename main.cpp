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
    init_pair(4, COLOR_BLACK, COLOR_WHITE);

    int maxes[2];
    getmaxyx(stdscr, maxes[0], maxes[1]);
    // Init game variables
    int cash = 1000;

    // Init windows
    enum Pages {menu, game};
    WINDOW *screens[2];
    screens[menu] = newwin(maxes[0]/4 + 2, maxes[1], 0, 0);
    screens[game] = newwin(maxes[0] - maxes[0]/4 - 2, maxes[1], maxes[0]/4 + 2, 0);

    const int TILE_SIZE = 7;
    const int WIDTH = TILE_SIZE * 2;
    int gamemaxes[2];
    
    getmaxyx(screens[game], gamemaxes[0], gamemaxes[1]);
    Tile tiles[gamemaxes[0] / TILE_SIZE][gamemaxes[1] / WIDTH];

    // Init timer
    auto t1 = high_resolution_clock::now();
    auto time = duration_cast<milliseconds>(t1.time_since_epoch());
    
    // Init tiles
    const int ROWS = 3;
    const int COLS = 10;
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

    // Init Termplates (the terms on the menu
    std::vector<Term> termplates;
    termplates.push_back(Term(0, 0, 0, 0));
    termplates.push_back(Term(1, 0, 1, 0));
    termplates.push_back(Term(2, 0, 2, 0));
    termplates.push_back(Term(3, 0, 3, 0));
    termplates.push_back(Term(4, 0, 4, 0));
    Cursor term_curse(0, 0, TILE_SIZE); 
    
    // Init bullet vector
    std::vector<Bullet> bullets;

    // Init cursor
    Cursor cursor(0, 0, TILE_SIZE); 

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
	if (cash >= termplates[type].cost) {
	    if (key == 't') {
		if (!tiles[cursor.pos[0]][cursor.pos[1]].occupied) {
		    cash -= termplates[type].cost;
		    Term t(type, cursor.pos[0], cursor.pos[1], time.count());
		    tiles[cursor.pos[0]][cursor.pos[1]].occupied = true;
		    terms.push_back(t);
		}
	    }
	}
	if (key == '1') type = 0;
	if (key == '2') type = 1;
	if (key == '3') type = 2;
	if (key == '4') type = 3;
	if (key == '5') type = 4;

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

	// Render menu bar
	for (auto term : termplates) {
	    // Show term
	    term.print(screens[menu]);
	    // Highlight the selected term
	    term_curse.pos[1] = type;
	    term_curse.print(screens[menu]);
	    // Render term name and cost
	    wattron(screens[menu], COLOR_PAIR(4));
	    mvwprintw(screens[menu], term.pos[0] - 1, (WIDTH - 9) / 2 + term.pos[1], "Cost: %i", term.cost);
	    mvwprintw(screens[menu], term.pos[0] + 7, (WIDTH - term.name.size()) / 2 + term.pos[1], term.name.data());
	    wattroff(screens[menu], COLOR_PAIR(4));
	}
	wattron(screens[menu], COLOR_PAIR(4));
	mvwprintw(screens[menu], 3, maxes[1] - 15, "Cash: %i", cash);
	wattroff(screens[menu], COLOR_PAIR(4));

	// Update and Render the Terms
	for (int i = 0; i < terms.size(); i++) {
	    // "Kill" the term
	    if (terms[i].health <= 0) terms.erase(terms.begin() + i);

	    int money_type = 4;
	    // Shoot bullets
	    if (time.count() >= terms[i].shoot_time + terms[i].cooldown) {
		terms[i].shoot_time = time.count();
		Bullet b(terms[i].bullet, terms[i].bullet_pos[0], terms[i].bullet_pos[1], bullets.size(), time.count());
		bullets.push_back(b);
		if (terms[i].type == 1) {
		    Bullet b(terms[i].bullet, terms[i].bullet_pos[0], terms[i].bullet_pos[1] - 3, bullets.size(), time.count());
		    bullets.push_back(b);
		}
		if (terms[i].type == 4) cash += 50;
	    }

	    // Render term
	    terms[i].print(screens[game]);
	}

	// Update and render the bullets
	for (int i = 0; i < bullets.size(); i++) {
	    // "Kill" bullet
	    if (bullets[i].pos[1] >= gamemaxes[1] || time.count() >= bullets[i].spawn_time + 4000) bullets.erase(bullets.begin() + i);

	    // Update the bullet
	    if (bullets[i].type != 4) bullets[i].update(time.count());

	    // Render the bullet
	    bullets[i].print(screens[game]);
	}

	// Update and render the cursor
	cursor.print(screens[screendex]);

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
