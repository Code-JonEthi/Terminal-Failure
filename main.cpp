#include <cstdlib>
#include <chrono>
#include <vector>
#include <curses.h>
#include <stdbool.h>
#include <thread>
#include "cursor.h"
#include "terms.h"
#include "worm.h"

using namespace std::chrono;
using namespace std::chrono_literals;

struct Tile {
    int pos[2];
    int size;
    int width;
    bool occupied;
    
    void print(WINDOW *screen) {
	for (int i = -1; i < size + 1; i ++) {
	    if (i == -1 or i == size) {
		mvwprintw(screen, pos[0] + i + 1, pos[1], "+--------------+");
	    } else {
		mvwprintw(screen, pos[0] + i + 1, pos[1], "|              |");
	    }
	}
    }
};

struct Wave {
    long last_spawn;
    int cooldown_range[2];
    int cooldown;
    int limit;
    int types;
    int spawned = 0;
    
    void spawn(long time, std::vector<Worm> *worms) {
	if (spawned <= limit) {
	    if (last_spawn + cooldown <= time) {
		int type = rand() % types;    
		int row = rand() % 3;
		last_spawn = time;
		spawned += 1;
		worms->push_back(Worm(row, type, time));
		cooldown = rand() % cooldown_range[1] + cooldown_range[0];
	    }
	}
    }

    Wave(long time, int cooldown_min, int cooldown_max, int limit, int types) {
	this->limit = limit;
	this->types = types;
	cooldown_range[0] = cooldown_min;
	cooldown_range[1] = cooldown_max;
	cooldown = cooldown_min;
	last_spawn = time;
    }
};

int main() {
    std::this_thread::sleep_for(0.6ms);
    srand(time(NULL));
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
    init_pair(5, COLOR_WHITE, COLOR_RED);

    int maxes[2];
    getmaxyx(stdscr, maxes[0], maxes[1]);
    // Init game variables
    int cash = 1000;

    // Init windows
    enum Pages {menu, game};
    enum State {play, pause, end, start};
    WINDOW *screens[2];
    screens[menu] = newwin(maxes[0]/4 + 2, maxes[1], 0, 0);
    screens[game] = newwin(maxes[0] - maxes[0]/4 - 2, maxes[1], maxes[0]/4 + 2, 0);
    int gamestate = start;
    bool won = false;

    const int TILE_SIZE = 7;
    const int WIDTH = TILE_SIZE * 2;
    int gamemaxes[2];
    
    getmaxyx(screens[game], gamemaxes[0], gamemaxes[1]);
    Tile tiles[3][10];

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
    std::vector<int> dead;

    // Init Worms
    std::vector<Worm> worms;

    // Init Waves
    std::vector<Wave> waves;
    waves.push_back(Wave(time.count(), 10000, 18000, 15, 2));
    waves.push_back(Wave(time.count(), 8000, 14000, 30, 3));
    waves.push_back(Wave(time.count(), 6000, 10000, 50, 3));
    waves.push_back(Wave(time.count(), 5000, 8000, 50, 4));
    waves.push_back(Wave(time.count(), 4000, 7000, 40, 4));
    int wave = 0;

    // Init cursor
    Cursor cursor(0, 0, TILE_SIZE); 

    int key;
    int screendex;
    screendex = game;
    bool running = true;
    while (running) {
	if (gamestate == play) {
	    // Get elapsed time
	    t1 = high_resolution_clock::now();
	    time = duration_cast<milliseconds>(t1.time_since_epoch());

	    // Check if current wave is over
	    if (waves[wave].spawned >= waves[wave].limit) {
		wave += 1;
		// End game if all waves are over
		if (wave >= waves.size()) {
		    running = false;
		    break;
		}
	    }

	    // Input handling
	    key = getch();
	    if (key == 'q') {
		running = false;
		break;
	    }
	    if (key == 'h') cursor.move(Cursor::left, ROWS, COLS);
	    if (key == 'j') cursor.move(Cursor::down, ROWS, COLS);
	    if (key == 'k') cursor.move(Cursor::up, ROWS, COLS);
	    if (key == 'l') cursor.move(Cursor::right, ROWS, COLS);
	    if (key == 'p') {
		gamestate = pause;
		continue;
	    }
	    if (cash >= termplates[type].cost) {
		if (key == 't') {
		    if (!tiles[cursor.pos[0]][cursor.pos[1]].occupied && termplates[type].last_placed + termplates[type].cooldown < time.count()) {
			termplates[type].last_placed = time.count();
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
	    term_curse.pos[1] = type;
	    for (int i = 0; i < termplates.size(); i++) {
		// Show term
		termplates[i].print(screens[menu]);
		// Highlight the selected term
		if (type == termplates[i].type) term_curse.print(screens[menu]);
		// Render term name, cost, and if it's on cooldown
		
		if (termplates[i].last_placed + termplates[i].cooldown >= time.count()) {
		    wattron(screens[menu], COLOR_PAIR(5));
		    mvwprintw(screens[menu], termplates[i].pos[0] + 3, termplates[i].pos[1] + 4, "COOLDOWN");
		    wattroff(screens[menu], COLOR_PAIR(5));
		}
		wattron(screens[menu], COLOR_PAIR(4));
		mvwprintw(screens[menu], termplates[i].pos[0] - 1, termplates[i].pos[1] + 1, "%i. Cost: %i", i + 1, termplates[i].cost);
		mvwprintw(screens[menu], termplates[i].pos[0] + 7, (WIDTH - termplates[i].name.size()) / 2 + termplates[i].pos[1], termplates[i].name.data());
		wattroff(screens[menu], COLOR_PAIR(4));
	    }
	    wattron(screens[menu], COLOR_PAIR(4));
	    mvwprintw(screens[menu], 2, maxes[1] - 15, "Wave: %i", wave);
	    mvwprintw(screens[menu], 1, maxes[1] - 15, "Spawned: %i", waves[wave].spawned);
	    mvwprintw(screens[menu], 3, maxes[1] - 15, "Cash: %i", cash);
	    mvwprintw(screens[menu], 3, maxes[1] - 40, "Worms: %li", worms.size());
	    wattroff(screens[menu], COLOR_PAIR(4));

	    // Update wave
	    waves[wave].spawn(time.count(), &worms);

	    // Update and Render the Terms
	    for (int i = 0; i < terms.size(); i++) {
		// "Kill" the term
		if (terms[i].health <= 0) terms.erase(terms.begin() + i);

		int money_type = 4;
		// Shoot bullets
		if (time.count() >= terms[i].shoot_time + terms[i].cooldown) {
		    terms[i].shoot_time = time.count();
		    Bullet b(terms[i].bullet, terms[i].row, terms[i].bullet_pos[0], terms[i].bullet_pos[1], bullets.size(), time.count());
		    bullets.push_back(b);
		    if (terms[i].type == 1) {
			Bullet b(terms[i].bullet, terms[i].row, terms[i].bullet_pos[0], terms[i].bullet_pos[1] - 3, bullets.size(), time.count());
			bullets.push_back(b);
		    }
		    if (terms[i].type == 4) cash += 50;
		}

		// Render term
		terms[i].print(screens[game]);
	    }

	    // Update and render the worms
	    for (int i = 0; i < worms.size(); i++) {
		worms[i].update(&terms, &tiles, time.count());
		worms[i].print(screens[game]);
		mvwprintw(screens[menu], 5, gamemaxes[1] - 18, "%li", worms[0].last_time);
	    }
	    mvwprintw(screens[menu], 6, gamemaxes[1] - 18, "Time: %li", time.count());

	    // Update and render the bullets
	    dead.clear();
	    for (int i = 0; i < bullets.size(); i++) {
		// "Kill" bullet
		if (bullets[i].pos[1] >= gamemaxes[1] || time.count() >= bullets[i].spawn_time + 4000) dead.push_back(i);

		// Update the bullet
		if (bullets[i].type != 4) bullets[i].update(time.count());

		// Check for bullet hits
		for (int j = 0; j < worms.size(); j++) {
		    if (bullets[i].pos[1] >= worms[j].pos[1] && bullets[i].row == worms[j].row) {
			worms[j].health -= bullets[i].damage;
			if (bullets[i].type == 1) {
			    worms[j].attr = 3;
			    worms[j].slowed = true;
			}
			if (dead.size() > 0 ? dead[dead.size() - 1] : -1 != i) dead.push_back(i);
			// Kill the worm
			if (worms[j].health <= 0) worms.erase(worms.begin() + j);
		    }
		}

		// Render the bullet
		bullets[i].print(screens[game]);
	    }

	    // Kill all "dead" bullets
	    for (int i = 0; i < dead.size(); i++) {
		bullets.erase(bullets.begin() + dead[i]);
	    }

	    // Update and render the cursor
	    cursor.print(screens[screendex]);

	}
	if (gamestate == pause) {
	    key = getch();
	    if (key == 'p') {
		gamestate = play;
	    }
	    if (key == 'q') {
		running = false;
		break;
	    }

	    wattron(screens[game], COLOR_PAIR(4));
	    mvwprintw(screens[game], gamemaxes[0]/2-3, gamemaxes[1]/2 - 3, "Paused");
	    mvwprintw(screens[game], gamemaxes[0]/2, gamemaxes[1]/2 - 4, "p - Play");
	    mvwprintw(screens[game], gamemaxes[0]/2+3, gamemaxes[1]/2 - 4, "q - Quit");
	    wattroff(screens[game], COLOR_PAIR(4));
	}
	if (gamestate == start) {
	    key = getch();
	    if (key == 'p') {
		gamestate = play;
	    }
	    if (key == 'q') {
		running = false;
		break;
	    }

	    wattron(screens[game], COLOR_PAIR(4));
	    mvprintw(maxes[0]/2 - 2, maxes[1]/2-7, "Terminal Failure!");
	    mvprintw(maxes[0]/2, maxes[1]/2-7, "Press 'p' to Play");
	    mvprintw(maxes[0]/2 + 2, maxes[1]/2-7, "Press 'q' to Quit");
	    wattroff(screens[game], COLOR_PAIR(4));
	}
	if (gamestate == end) {
	    key = getch();
	    if (key != ERR) running = false;

	    wattron(screens[game], COLOR_PAIR(4));
	    if (won) mvwprintw(screens[game], gamemaxes[0]/2-2, gamemaxes[1]/2 - 5, "Game Over :(");
	    else mvwprintw(screens[game], gamemaxes[0]/2-2, gamemaxes[1]/2 - 5, "You Win! :)");
	    mvwprintw(screens[game], gamemaxes[0]/2, gamemaxes[1]/2 - 9, "Press Any Key To Quit.");
	    wattroff(screens[game], COLOR_PAIR(4));
	}

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
