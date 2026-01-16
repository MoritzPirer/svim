#include "../../inc/Model/EditorState.hpp"

void EditorState::moveCursorUp() {
    if (m_cursor.getPosition().row > 0) {
        size_t new_line_length = m_current_file.getLine(m_cursor.getPosition().row - 1).length();
        m_cursor.moveUp(new_line_length);
    }
}

void EditorState::moveCursorLeft() {
    if (m_cursor.getPosition().column > 0) {
        m_cursor.moveLeft();
    }
}

void EditorState::moveCursorDown() {
    if (m_cursor.getPosition().row + 1 < m_current_file.getLineCount()) {
        size_t new_line_length = m_current_file.getLine(m_cursor.getPosition().row + 1).length();
        m_cursor.moveDown(new_line_length);
    }
}

void EditorState::moveCursorRight() {
    if (m_cursor.getPosition().column + 1 < m_current_file.getLine(m_cursor.getPosition().row).length()) {
        m_cursor.moveRight();
    }
}

size_t EditorState::getFirstVisibleLine(int screen_width, int screen_height) {
    /*
        from the cursor upwards, calculate visual rows from logical rows.
        if visual rows = screen height / 2 before we run out of rows, return the last line needed
        otherwise, return first line
    */
    int visual_lines_above_cursor = 0;
    for (size_t i = 0; i < m_cursor.getPosition().row; i++) {
        size_t logical_line = m_cursor.getPosition().row - i - 1; //this avoids underflow in loop condition

        visual_lines_above_cursor += m_current_file.visualLinesOfLine(logical_line, screen_width);
        if (visual_lines_above_cursor * 2 >= screen_height) {
            return i;
        }
    }

    return 0;
}
