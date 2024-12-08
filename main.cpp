#include <iostream>
#include <vector>
#include <stdbool.h>
#include <stdlib.h>
#include <curses.h>
#include "screens.h"
#include "cursor.h"

using namespace std;

int main() {
    // Initialize NCurses and it's settings
    initscr();
    start_color();
    cbreak();
    noecho();
    timeout(0);
    curs_set(0);

    // Variable to keep track of keyboard presses
    char key;

    const int tile_size = 7;
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // Initialize color pairs
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    attron(COLOR_PAIR(1));

    // Initialize the 3 different screens and add them to a vector
    // Naming convention for these is horrible I know.
    vector<Screen> screens;
    // Enumerator to easily keep track of the indexes of the different screens
    enum Pages {menu, game, tooltips};
    screens.push_back(Screen("Menu", maxy/4, maxx, 0, 0));
    screens.push_back(Screen("Game", maxy/2 + maxy/6, maxx, maxy/4-1, 0));
    screens.push_back(Screen("Tooltips", maxy/6, maxx, maxy-maxy/6, 0));
    nodelay(screens.at(menu).screen, true);
    nodelay(screens.at(game).screen, true);
    nodelay(screens.at(tooltips).screen, true);
    
    // Initialize the cursor to visually select tiles on the grid
    Cursor cursor(2, 2, tile_size - 2);

    // screendex = screen index, used to fetch screens from the vector
    int screendex;
    screendex = menu;

    // Main gameplay loop
    bool running = true;
    while(running) {

	// Input fetching
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
	    wgetch(screen.screen);
	    wrefresh(screen.screen);
	}

	// Draw all other elements
	// Render the cursor last
	cursor.draw(screens.at(screendex).screen);
    }

    // Turn of the color attribute and end the windows
    attroff(COLOR_PAIR(1));
    endwin();
    return 0;
}
