#include <ncurses.h>

#include "../../inc/View/NcursesSession.hpp"

NcursesSession::NcursesSession() {
    initscr();
    if (!stdscr) {
        return;
    } 
    initialized = true;
    raw();
    keypad(stdscr, TRUE);
    noecho();
    set_escdelay(25);
    curs_set(0);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
    
    start_color();
    use_default_colors(); 

    init_pair(1, COLOR_WHITE,   -1); // Text Normal
    init_pair(2, COLOR_MAGENTA, -1); // Text Highlight
    init_pair(3, COLOR_YELLOW,  -1); // changed file
    init_pair(4, COLOR_RED,     -1); // new file
    init_pair(5, COLOR_GREEN,   -1); // saved file
    init_pair(6, COLOR_BLUE,    -1); // ui element
    init_pair(7, COLOR_BLACK, COLOR_WHITE); // strong highlight
}

NcursesSession::~NcursesSession() {
    if (!initialized) { 
        return;
    }
    curs_set(1);
    endwin();
}