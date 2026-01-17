#include "../../inc/Controller/EditorController.hpp"

size_t EditorController::getCursorRow() const {
    return m_state.getCursor().getPosition().row;
}

size_t EditorController::getCursorColumn() const {
    return m_state.getCursor().getPosition().column;
}

bool EditorController::processInput(int input, int screen_width) {
    switch (input) {
        case 'h':
            m_state.moveCursorLeft();
            break;
        case 'j':
            m_state.moveCursorDown(screen_width);
            break;
        case 'k':
            m_state.moveCursorUp(screen_width);
            break;
        case 'l':
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

size_t EditorController::getFirstVisibleLine(int screen_width, int screen_height) {
    return m_state.getFirstVisibleLine(screen_width, screen_height);
}

size_t EditorController::getLineCount() const {
    return m_state.getLineCount();
}
const std::string& EditorController::getLine(size_t row) const {
    return m_state.getLine(row);
}