#include <iostream>
#include <vector>
#include <stdbool.h>
#include <stdlib.h>
#include <curses.h>
#include "screens.h"
#include "cursor.h"

void render_tiles(WINDOW *screen, int size);

int main() {
    // Initialize NCurses and it's settings
    initscr();
    start_color();
    cbreak();
    noecho();
    timeout(0);
    curs_set(0);
    refresh();

    // Variable to keep track of keyboard presses
    char key;

    const int TILE_SIZE = 7;
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // Initialize color pairs
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    attron(COLOR_PAIR(1));

    // Initialize the 3 different screens and add them to a vector
    // Naming convention for these is horrible I know.
    std::vector<Screen> screens;
    // Enumerator to easily keep track of the indexes of the different screens
    enum Pages {menu, game, tooltips};
    screens.push_back(Screen("Menu", maxy/4, maxx, 0, 0));
    screens.push_back(Screen("Game", maxy/2 + maxy/6, maxx, maxy/4-1, 0));
    screens.push_back(Screen("Tooltips", maxy/6, maxx, maxy-maxy/6, 0));
    nodelay(screens.at(menu).screen, true);
    nodelay(screens.at(game).screen, true);
    nodelay(screens.at(tooltips).screen, true);
    
    // Initialize the cursor to visually select tiles on the grid
    Cursor cursor(2, 2, TILE_SIZE - 2);

    // screendex = screen index, used to fetch screens from the vector
    int screendex;
    screendex = menu;

    // Main gameplay loop
    bool running = true;
    while(running) {

	// Input fetching
	mvprintw(5, 5, "Test");
	refresh();
	key = wgetch(screens.at(screendex).screen);
	switch (key) {
	    // Switch pages with TAB
	    case '\t':
		screendex = screendex < tooltips ? screendex < game ? game : tooltips : menu;
		cursor.next_tab();
		break;
	    // Quit game with q
	    case 'q':
		running = false;
		break;
	    // Vim Motions for cursor movement
	    case 'h':
		cursor.move(cursor.left, screens.at(screendex).screen);
		break;
	    case 'j':
		cursor.move(cursor.down, screens.at(screendex).screen);
		break;
	    case 'k':
		cursor.move(cursor.up, screens.at(screendex).screen);
		break;
	    case 'l':
		cursor.move(cursor.right, screens.at(screendex).screen);
		break;
	    // literally nothing idk why this is here
	    default: continue;
	}

	// Clear and redraw each screen
	for (Screen screen : screens) {
	    werase(screen.screen);
	    screen.draw();
	    wnoutrefresh(screen.screen);
	}

	// Draw all other elements
	render_tiles(screens.at(game).screen, TILE_SIZE);
	// Render the cursor last
	cursor.draw(screens.at(screendex).screen);

	doupdate();
    }

    // Turn of the color attribute and end the windows
    attroff(COLOR_PAIR(1));
    for (Screen page : screens) {
	delwin(page.screen);
    }
    endwin();
    return 0;
}

void render_tiles(WINDOW *screen, int size) {
    int maxy, maxx, width;
    getmaxyx(screen, maxy, maxx);
    width = size + size / 2 - 2;

    for (int i = 1; i < maxy - 1; i += size - 1) {
	for (int j = 1; j < maxx - 1; j += width) {
	    mvwprintw(screen, i, j, "0");
	}
    }
}
