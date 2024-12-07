#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

using namespace std;
int main() {
	initscr();
	start_color();
	noecho();
	curs_set(0);
	keypad(stdscr, true);

	int maxy, maxx;
	getmaxyx(stdscr, maxy, maxx);

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	WINDOW *menu = newwin(maxy/4, maxx, 0, 0);
	WINDOW *tooltips = newwin(maxy/6, maxx, maxy-maxy/6, 0);
	WINDOW *test = newwin(maxy/2, maxx/2, maxy/4, maxx/4);

	box(menu, 0, 0);
	box(test, 0, 0);
	box(tooltips, 0, 0);

	mvwprintw(menu, 0, 1, "Menu");
	mvwprintw(tooltips, 0, 1, "Descriptions");

	wnoutrefresh(menu);
	wnoutrefresh(test);
	wnoutrefresh(tooltips);
	doupdate();

	getch();

	endwin();
	return 0;
}
