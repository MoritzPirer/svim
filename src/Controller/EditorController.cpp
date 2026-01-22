#include <ncurses.h>
#include "../../inc/Controller/EditorController.hpp"
#include "../../inc/Controller/FileHandler.hpp"

EditorController::EditorController(std::optional<std::string> file_path) {
    FileHandler file_handler;
    TextFile file;
    if (!file_path.has_value() || file_path->empty()) {
        std::filesystem::path name = file_handler.getDefaultName(); 
        file = file_handler.createFile(name);
    }
    else {
        file = file_handler.openFile(file_path.value());      
    }
    m_state = EditorState(file);
}

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

int EditorController::screenLineOfCursor(int screen_width, int screen_height) {
    int visual_line = m_state.calculateVisualLineOfCursor(screen_width);


    return std::min(screen_height / 2 - 1, visual_line - 1);
}