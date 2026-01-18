#include <ncurses.h>
#include "../../inc/Controller/EditorController.hpp"

int EditorController::getCursorRow() const {
    return m_state.getCursor().getPosition().row;
}

int EditorController::getCursorColumn() const {
    return m_state.getCursor().getPosition().column;
}

bool EditorController::processInput(int input, int screen_width) {
    switch (input) {
        case 'h':
        case KEY_LEFT:
            m_state.moveCursorLeft();
            break;
        case 'j':
        case KEY_DOWN:
            m_state.moveCursorDown(screen_width);
            break;
        case 'k':
        case KEY_UP:
            m_state.moveCursorUp(screen_width);
            break;
        case 'l':
        case KEY_RIGHT:
            m_state.moveCursorRight();
            break;
        case 'w':
            for (size_t _ = 0; _ < 10; _++) {
                m_state.moveCursorRight();
            }
            break;
        case 'q':
            return true;
        default:
            break;
    }
    
    return false;
}

Position EditorController::getFirstVisibleChar(int screen_width, int screen_height) {
    return m_state.getFirstVisibleChar(screen_width, screen_height);
}

int EditorController::getLineCount() const {
    return m_state.getLineCount();
}

std::string EditorController::getPartialLine(Position start) {
    return m_state.getPartialLine(start);
}

const std::string& EditorController::getLine(size_t row) const {
    return m_state.getLine(row);
}