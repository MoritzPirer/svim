#include <ncurses.h>

#include "../../inc/View/Display.hpp"

void Display::render() {
    for (size_t i = 0; i < m_controller.getLineCount(); i++) {
        // fprintf(stderr, "in render with i = %zu/%zu\n", i, m_controller.getLineCount());
        // fflush(stderr);

        mvprintw(i, 0, "%s", m_controller.getLine(i).c_str());
    }

    size_t cursor_row = m_controller.getCursorRow();
    size_t cursor_column = m_controller.getCursorColumn();
    mvprintw(10, 0, "cursor at (%zu/%zu)", cursor_row, cursor_column);
    move(cursor_row, cursor_column);
    curs_set(2);


    refresh();
}

void Display::mainLoop() {
    bool quit = false;

    while (!quit) {
        render();

        int input = getch();
        quit = m_controller.processInput(input);
    }
}