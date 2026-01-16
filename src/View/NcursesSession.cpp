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
    curs_set(0);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
}

NcursesSession::~NcursesSession() {
    if (!initialized) { 
        return;
    }
    curs_set(1);
    endwin();
}