#include <ncurses.h>

#include "../../inc/View/NcursesSession.hpp"

NcursesSession::NcursesSession() {
  initscr();
  if (stdscr == nullptr) {
    return; // In case 'initscr()' fails
  }
  raw();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  //uncomment this line to prevent mouse wheel being treated as arrow keys. this disables scrolling
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
}

NcursesSession::~NcursesSession() {
    curs_set(1);
    endwin();
}