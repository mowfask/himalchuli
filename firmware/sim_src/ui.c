#include "ui.h"
#include <ncurses.h>

static void print_template(void)
//Assuming window size of 80x24
{
    int i;

    //Headings and divisions:
    attron(A_BOLD | A_UNDERLINE);
    mvprintw(0, 1, "%s", "Outputs");
    mvprintw(0, 40, "%s", "Inputs");
    mvprintw(19, 1, "%s", "Controls");
    attroff(A_UNDERLINE);
    for(i=0; i<80; ++i)
    {
        mvaddch(18, i, '#');
    }
    for(i=0; i<18; ++i)
    {
        mvaddch(i, 37, '#');
    }
    attroff(A_BOLD);

    //Outputs
    mvprintw(2, 3, "%s", "Motor:");
    mvprintw(3, 3, "%s", "LED green:");
    mvprintw(4, 3, "%s", "LED red:");

    mvprintw(2, 18, "%s", "off  "); //"left ","right"
    mvprintw(3, 18, "%s", "off");   //"on "
    mvprintw(4, 18, "%s", "off");   //"on "

    //Inputs
    mvprintw(2, 42, "%s", "Manual enable:");
    mvprintw(3, 42, "%s", "Manual up:");
    mvprintw(4, 42, "%s", "Manual down:");
    mvprintw(6, 42, "%s", "Motor rotation:");
    mvprintw(7, 42, "%s", "Current:");
    mvprintw(8, 42, "%s", "Distance:");

    mvprintw(2, 60, "%s", "released");  //"pressed "
    mvprintw(3, 60, "%s", "released");  //"pressed "
    mvprintw(4, 60, "%s", "released");  //"pressed "
    mvprintw(6, 60, "%s", "released");  //"pressed "
    mvprintw(7, 60, "%s", "  0");       //"  1" ... "255"
    mvprintw(8, 60, "%s", "  0");       //"  1" ... "255"

    //Controls
    mvprintw(19, 11, "%s", "e    : Manual enable");
    mvprintw(20, 11, "%s", "UP   : Manual up");
    mvprintw(21, 11, "%s", "DOWN : Manual down");
    mvprintw(19, 35, "%s", "r : Motor rotation");
    mvprintw(20, 35, "%s", "q : Current+");
    mvprintw(21, 35, "%s", "a : Current-");
    mvprintw(22, 35, "%s", "w : Distance+");
    mvprintw(23, 35, "%s", "s : Distance-");

    mvaddch(23,0,'>');
    refresh();
}

void ui_init(void)
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    print_template();
    getch();
}

void ui_deinit(void)
{
    endwin();
}

