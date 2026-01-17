#include "../../inc/Model/EditorState.hpp"
#include <ncurses.h>

void EditorState::moveCursorUp(int screen_width) {
    bool is_first_logical_line = m_cursor.getRow() == 0;
    bool is_first_visual_line = m_cursor.getColumn() < screen_width;

    if (is_first_logical_line && is_first_visual_line) {
        m_cursor.setColumn(0);
        return;
    }

    if (is_first_visual_line) {
        m_cursor.moveUpLogical();
        
        int new_column = (m_current_file.getLineLength(m_cursor.getRow()) / screen_width ) * screen_width + m_cursor.getColumn();
        m_cursor.setColumn(new_column);
        m_cursor.setColumn(std::min(
            m_cursor.getColumn(),
            m_current_file.getLineLength(m_cursor.getRow())
        ));
    }
    else { //not in first visual line -> move to previous visual line of same logical line
        m_cursor.moveUpVisual(screen_width);
    }

}


void EditorState::moveCursorDown(int screen_width) {
    bool is_last_logical_line = m_cursor.getRow() == m_current_file.getLineCount() - 1;
    
    int line_length = m_current_file.getLineLength(m_cursor.getRow());
    bool is_last_visual_line = (line_length == 0)
    || (m_cursor.getColumn() / screen_width == (line_length - 1) / screen_width); 
    
    if (is_last_logical_line && is_last_visual_line) { // if in last visual line of last logical line, move to very end
        m_cursor.setColumn(m_current_file.getLineLength(m_cursor.getRow()) - 1); //MODO HANDLE LAST LINE IS EMPTY
        return;
    }

    if (is_last_visual_line) { // if in last visual line of logical line: move to next logical line (attempt same col)
        m_cursor.moveDownLogical();
        m_cursor.setColumn(m_cursor.getColumn() % screen_width);
    }
    else { // if not in last visual line of logical line: move to next visual line of logical line (attempt same col)
        m_cursor.moveDownVisual(screen_width);
    }

    int new_line_length = m_current_file.getLineLength(m_cursor.getRow());

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
        int new_line_length = m_current_file.getLineLength(m_cursor.getRow());
        m_cursor.setColumn(new_line_length == 0? 0 : new_line_length - 1);
    }
}

void EditorState::moveCursorRight() {
    int line_length = m_current_file.getLine(m_cursor.getRow()).length();
    if (m_cursor.getPosition().column + 1 < line_length) { // normal move right
        m_cursor.moveRight();
    }
    else if (m_cursor.getRow() + 1 < m_current_file.getLineCount()) { // end of line -> jump to start of next
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

    //     visual_lines_above_cursor += m_current_file.visualLinesOfLine(logical_line, screen_width);
    //     if (visual_lines_above_cursor * 2 >= screen_height) {
    //         return i;
    //     }
    // }

    return 0 * screen_height * screen_width;
}
