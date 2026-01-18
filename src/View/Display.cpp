#include <ncurses.h>

#include "../../inc/View/Display.hpp"
#include "../../inc/Model/TextFile.hpp"

int Display::screenHeight() {
    return getmaxy(stdscr);
}
int Display::screenWidth() {
    return getmaxx(stdscr);
}

int Display::visualRowsNeeded(size_t line_length) {
    if (line_length == 0) {
        return 1;
    }
    // bool has_partial_line = line_length % (screenWidth() - 1) != 0;

    // return line_length / screenWidth() + (has_partial_line? 1 : 0);
    int lines_needed = (line_length + screenWidth() - 1) / screenWidth();
    return lines_needed + (line_length % screenWidth() == 0);
}
    

 
void Display::renderCursor(size_t visual_row_of_cursor) {
    size_t cursor_row = m_controller.getCursorRow();
    size_t cursor_column = m_controller.getCursorColumn();

    size_t screen_width = static_cast<size_t>(getmaxx(stdscr));
    // DEBUG HELPER
    //mvprintw(screenHeight() - 2, 0, "cursor at (%zu/%zu) (sw = %zu)", cursor_row, cursor_column, screen_width);
    
    while (cursor_column >= screen_width) {
        cursor_column -= screen_width;
        cursor_row++;
    }
    

    
    move(visual_row_of_cursor, cursor_column);
    curs_set(2);
}

void Display::render() {
    clear();
    Position first_visible_char = m_controller.getFirstVisibleChar(screenWidth(), screenHeight());

    const std::string& first_visible_line = m_controller.getPartialLine(first_visible_char);
    mvprintw(0, 0, "%s", first_visible_line.c_str());
    
    size_t visual_row_of_cursor = 0;
    if (m_controller.getCursorRow() == 0) {
        visual_row_of_cursor = visualRowsNeeded(m_controller.getCursorColumn()) - 1;
    }

    int logical_line_index = first_visible_char.row + 1;
    for (int visual_row = visualRowsNeeded(first_visible_line.length());
        visual_row < screenHeight();) {

        if (logical_line_index >= m_controller.getLineCount()) {
            mvprintw(visual_row, 0, "~");
            visual_row++;
        }
        else {
            const std::string& line = m_controller.getLine(logical_line_index);
            mvprintw(visual_row, 0, "%s", line.c_str());
            if (m_controller.getCursorRow() == logical_line_index) {
                visual_row_of_cursor = visual_row + visualRowsNeeded(m_controller.getCursorColumn()) - 1;
            }
            
            visual_row += visualRowsNeeded(line.length());
            logical_line_index++;
        }
    }

    // for (int visual_row = 0; visual_row < screenHeight();) {
    //     if (logical_line_index >= m_controller.getLineCount()) {
    //         mvprintw(visual_row, 0, "~");
    //         visual_row++;
    //     }
    //     else {
    //         const std::string& line = m_controller.getLine(logical_line_index);
    //         mvprintw(visual_row, 0, "%s", line.c_str());
    //         if (m_controller.getCursorRow() == logical_line_index) {
    //             visual_row_of_cursor = visual_row + visualRowsNeeded(m_controller.getCursorColumn()) - 1;
    //         }
            
    //         visual_row += visualRowsNeeded(line.length());
    //         logical_line_index++;
    //     }

    // }

    renderCursor(visual_row_of_cursor);

    refresh();
}

void Display::mainLoop() {
    bool quit = false;

    while (!quit) {
        render();

        int input = getch();
        quit = m_controller.processInput(input, screenWidth());
    }
}