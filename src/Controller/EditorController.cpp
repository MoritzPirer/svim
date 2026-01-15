#include "../../inc/Controller/EditorController.hpp"

size_t EditorController::getCursorRow() const {
    return m_state.getCursor().getRow();
}

size_t EditorController::getCursorColumn() const {
    return m_state.getCursor().getColumn();
}

bool EditorController::processInput(int input) {
    switch (input) {
        case 'h':
            m_state.moveCursorLeft();
            break;
        case 'j':
            m_state.moveCursorDown();
            break;
        case 'k':
            m_state.moveCursorUp();
            break;
        case 'l':
            m_state.moveCursorRight();
            break;
        
        default:
            return true;
    }
    
    return false;
}

size_t EditorController::getLineCount() const {
    return m_state.getLineCount();
}
const std::string& EditorController::getLine(size_t row) const {
    return m_state.getLine(row);
}