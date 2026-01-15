#include "../../inc/Model/EditorState.hpp"

void EditorState::moveCursorUp() {
    if (m_cursor.getRow() > 0) {
        size_t new_line_length = m_current_file.getLine(m_cursor.getRow() - 1).length();
        m_cursor.moveUp(new_line_length);
    }
}

void EditorState::moveCursorLeft() {
    if (m_cursor.getColumn() > 0) {
        m_cursor.moveLeft();
    }
}

void EditorState::moveCursorDown() {
    if (m_cursor.getRow() + 1 < m_current_file.getLineCount()) {
        size_t new_line_length = m_current_file.getLine(m_cursor.getRow() + 1).length();
        m_cursor.moveDown(new_line_length);
    }
}

void EditorState::moveCursorRight() {
    if (m_cursor.getColumn() + 1 < m_current_file.getLine(m_cursor.getRow()).length()) {
        m_cursor.moveRight();
    }
}
