#include "../../inc/Model/EditorState.hpp"

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
    bool is_last_logical_line = (m_cursor.getRow() == m_file.getLineCount() - 1);
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

    if (m_cursor.getColumn() >= new_line_length) {
        m_cursor.setColumn(std::max(new_line_length - 1, 0));
    }
}

void EditorState::moveCursorLeft() {
    // normal move left
    if (m_cursor.getPosition().column > 0) { 
        m_cursor.moveLeft();
    }
    // start of line -> jump to end of previous
    else if (m_cursor.getPosition().row > 0) { 
        m_cursor.moveUpLogical();
        
        int new_line_length = m_file.getLineLength(m_cursor.getRow());
        m_cursor.setColumn(std::max(new_line_length - 1, 0));
    }
}

void EditorState::moveCursorRight() {
    int line_length = m_file.getLine(m_cursor.getRow()).length();

    // normal move right
    if (m_cursor.getPosition().column + 1 < line_length) { 
        m_cursor.moveRight();
    }
    // end of line -> jump to start of next
    else if (m_cursor.getRow() + 1 < m_file.getLineCount()) { 
        m_cursor.moveDownLogical();
        m_cursor.setColumn(0);
    }
}

int EditorState::calculateVisualLineOfCursor(int screen_width) const {
    int visual_line_of_cursor = TextFile::visualLinesNeeded(m_cursor.getColumn(), screen_width);
    
    for (int logical_line_index = 0; logical_line_index < m_cursor.getRow(); logical_line_index++) {
        visual_line_of_cursor += m_file.visualLinesOfLine(logical_line_index, screen_width);
    }

    return visual_line_of_cursor;
}

Position EditorState::skipOffscreenLines(int offscreen_visual_lines, int screen_width) const {
    for (int logical_line_index = 0; logical_line_index < m_cursor.getRow(); logical_line_index++) {
        int line_length = m_file.getLineLength(logical_line_index);
        int visual_rows = TextFile::visualLinesNeeded(line_length, screen_width);

        // still more to skip
        if (visual_rows <= offscreen_visual_lines) {
            offscreen_visual_lines -= visual_rows;
            continue;
        }

        return {logical_line_index, offscreen_visual_lines * screen_width};
    }

    // should be unreachable if offscreen_visual_lines is correct
    throw std::runtime_error("something went wrong in getFirstVisibleChar()");
}

Position EditorState::getFirstVisibleChar(int screen_width, int screen_height) {
    int visual_line_of_cursor = calculateVisualLineOfCursor(screen_width);

    // too far up to place cursor in the middle
    if (visual_line_of_cursor <= screen_height / 2) {
        return {0,0};
    }

    // skip some lines to keep cursor in the middle
    int offscreen_visual_lines = visual_line_of_cursor - (screen_height / 2);
    
    return skipOffscreenLines(offscreen_visual_lines, screen_width);
}

std::string EditorState::getPartialLine(Position start) {
    return m_file.getLine(start.row).substr(start.column);
}
