#include <ncurses.h>

#include "../../inc/View/Display.hpp"
#include "../../inc/Model/TextFile.hpp"

int Display::screenHeight() {
    return getmaxy(stdscr);
}
int Display::screenWidth() {
    return getmaxx(stdscr);
}

void Display::renderLine(int& visual_row, const std::string& line) {
    mvprintw(visual_row, 0, "%s", line.c_str());
    visual_row += TextFile::visualLinesNeeded(line.length(), screenWidth());
}

void Display::renderEmptyLine(int& visual_row) {
    std::string empty_line = "~"; // FUTURE: READ THIS FROM SETTINGS VIA CONTROLLER
    renderLine(visual_row, empty_line);
}

void Display::renderText() {
    // first visible line
    int visual_row = 0;    
    int logical_line_index = 0;
    if (m_controller.getLineCount() > 0) {
        Position first_visible_char = m_controller.getFirstVisibleChar(screenWidth(), screenHeight());
        const std::string& first_visible_line = m_controller.getPartialLine(first_visible_char);
        renderLine(visual_row, first_visible_line);
        logical_line_index = first_visible_char.row + 1;
    }

    // rest of screen
    while (visual_row < screenHeight()) {
        if (logical_line_index >= m_controller.getLineCount()) {
            renderEmptyLine(visual_row);
            continue;
        }
        
        renderLine(visual_row, m_controller.getLine(logical_line_index));

        logical_line_index++;
    }
}
 
void Display::renderCursor() {
    int cursor_column = m_controller.getCursorColumn();
    int screen_row_of_cursor = m_controller.screenLineOfCursor(screenWidth(), screenHeight());

    // DEBUG HELPER
    //size_t cursor_row = m_controller.getCursorRow();
    //mvprintw(screenHeight() - 2, 0, "cursor at (%zu/%zu) (sw = %zu)", cursor_row, cursor_column, screenWidth());

    while (cursor_column >= screenWidth()) {
        cursor_column -= screenWidth();
    }
    
    move(screen_row_of_cursor, cursor_column);
    curs_set(2);
}

void Display::render() {
    clear();

    renderText();
    renderCursor();
    // renderMetadata() here, file numbers probably in renderText() or hidden from display if possible

    refresh();
}

Display::Display(const std::string& file_path):
    m_ncurses_session{},
    m_controller{file_path}
    {}

Display::Display():
    m_ncurses_session{},
    m_controller{std::nullopt}
    {}

void Display::mainLoop() {
    bool quit = false;

    while (!quit) {
        render();

        int input = getch();
        quit = m_controller.processInput(input, screenWidth());
    }
}