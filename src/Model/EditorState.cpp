#include "../../inc/Model/EditorState.hpp"
#include <ncurses.h>

bool EditorState::isLastVisualLineOfLine(int screen_width) {
    int line_length = m_file.getLineLength(m_cursor.getRow());
    
    if (line_length == 0) {
        return true;
    }
    
    return m_cursor.getColumn() / screen_width
        == (line_length - 1) / screen_width;
}

void EditorState::moveCursorUp(int screen_width) {
    bool is_first_logical_line = m_cursor.getRow() == 0;
    bool is_first_visual_line = m_cursor.getColumn() < screen_width;

    // case 1: already in the very first visual line 
    if (is_first_logical_line && is_first_visual_line) {
        m_cursor.setColumn(0);
        return;
    }

    // case 2: logical line has visual line above current visual line
    if (!is_first_visual_line) {
        m_cursor.moveUpVisual(screen_width);
        return;
    }

    // case 3: in the first visual line of the logical line
    m_cursor.moveUpLogical();
    
    int new_column = (m_file.getLineLength(m_cursor.getRow()) / screen_width )
        * screen_width + m_cursor.getColumn();

    m_cursor.setColumn(std::min(
        new_column,
        m_file.getLineLength(m_cursor.getRow())
    ));
}


void EditorState::moveCursorDown(int screen_width) {
    bool is_last_logical_line = m_cursor.getRow() == m_file.getLineCount() - 1;
    bool is_last_visual_line = isLastVisualLineOfLine(screen_width);
    
    // case 1: already in last visual line of file
    if (is_last_logical_line && is_last_visual_line) { 
        m_cursor.setColumn(std::max(m_file.getLineLength(m_cursor.getRow()) - 1, 0 )); 
        return;
    }

    // case 2: last visual line of logical line
    if (is_last_visual_line) { 
        m_cursor.moveDownLogical();
        m_cursor.setColumn(m_cursor.getColumn() % screen_width);
    }
    // case 3: logical line has visual line below current
    else { 
        m_cursor.moveDownVisual(screen_width);
    }

    int new_line_length = m_file.getLineLength(m_cursor.getRow());

    m_cursor.setColumn(std::min(
        m_cursor.getColumn(),
        new_line_length == 0? 0 : new_line_length - 1
    ));
}

void EditorState::moveCursorLeft() {
    if (m_cursor.getPosition().column > 0) { // normal move left
        m_cursor.moveLeft();
    }
    else if (m_cursor.getPosition().row > 0) { // start of line -> jump to end of previous
        m_cursor.moveUpLogical();
        
        int new_line_length = m_file.getLineLength(m_cursor.getRow());
        m_cursor.setColumn(new_line_length == 0? 0 : new_line_length - 1);
    }
}

void EditorState::moveCursorRight() {
    int line_length = m_file.getLine(m_cursor.getRow()).length();

    if (m_cursor.getPosition().column + 1 < line_length) { // normal move right
        m_cursor.moveRight();
    }
    else if (m_cursor.getRow() + 1 < m_file.getLineCount()) { // end of line -> jump to start of next
        m_cursor.moveDownLogical();
        m_cursor.setColumn(0);
    }
}

size_t EditorState::getFirstVisibleLine(int screen_width, int screen_height) {
    /*
        from the cursor upwards, calculate visual rows from logical rows.
        if visual rows = screen height / 2 before we run out of rows, return the last line needed
        otherwise, return first line
    */
    // int visual_lines_above_cursor = 0;
    // for (int i = 0; i < m_cursor.getRow(); i++) {
    //     int logical_line = m_cursor.getRow() - i - 1; //this avoids underflow in loop condition

    //     visual_lines_above_cursor += m_file.visualLinesOfLine(logical_line, screen_width);
    //     if (visual_lines_above_cursor * 2 >= screen_height) {
    //         return i;
    //     }
    // }

    return 0 * screen_height * screen_width;
}
