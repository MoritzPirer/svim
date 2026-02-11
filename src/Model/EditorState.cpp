#include "../../inc/Model/EditorState.hpp"

#include <ncurses.h>

bool EditorState::isCursorInLastRowOfParagraph(int screen_width) {
    int line_length = m_file.getParagraphLength(m_cursor.getRow());
    
    if (line_length == 0) {
        return true;
    }
    
    return m_cursor.getColumn() / screen_width == (line_length - 1) / screen_width;
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
    
    int new_column = (m_file.getParagraphLength(m_cursor.getRow()) / screen_width )
        * screen_width + m_cursor.getColumn();

    m_cursor.setColumn(std::min(
        new_column,
        m_file.getParagraphLength(m_cursor.getRow())
    ));
}


void EditorState::moveCursorDown(int screen_width) {
    bool is_last_logical_line = (m_cursor.getRow() == m_file.getNumberOfParagrahps() - 1);
    bool is_last_visual_line = isCursorInLastRowOfParagraph(screen_width);
    
    // case 1: already in last visual line of file
    if (is_last_logical_line && is_last_visual_line) { 
        m_cursor.setColumn(std::max(m_file.getParagraphLength(m_cursor.getRow()), 0 )); 
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

    int new_line_length = m_file.getParagraphLength(m_cursor.getRow());

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
        
        int new_line_length = m_file.getParagraphLength(m_cursor.getRow());
        m_cursor.setColumn(std::max(new_line_length, 0));
    }
}

void EditorState::moveCursorRight() {
    int line_length = m_file.getParagraph(m_cursor.getRow()).length();

    // normal move right
    if (m_cursor.getPosition().column < line_length) { 
        m_cursor.moveRight();
    }
    // end of line -> jump to start of next
    else if (m_cursor.getRow() + 1 < m_file.getNumberOfParagrahps()) { 
        m_cursor.moveDownLogical();
        m_cursor.setColumn(0);
    }
}

void EditorState::moveCursor(Direction direction, int screen_width) {
    switch (direction) {
    case Direction::UP: {
        moveCursorUp(screen_width);
        return;
    }

    case Direction::RIGHT: {
        moveCursorRight();
        return;
    }

    case Direction::DOWN: {
        moveCursorDown(screen_width);
        return;
    }

    case Direction::LEFT: {
        moveCursorLeft();
        return;
    }

    default: {
        break;
    }

    }
}

void EditorState::moveCursorTo(Position position) {
    if (!m_file.isValidCursorPosition(position)) {
        throw new std::invalid_argument("Invalid move destination " + position.format() + "!");
    }

    m_cursor.setColumn(position.column);
    m_cursor.setRow(position.row);
}

bool EditorState::canCursorMove(Direction direction) const {
    switch (direction) {
    case Direction::UP: {
        return (m_cursor.getRow() != 0); //TODO cover edge cases if needed
    }

    case Direction::DOWN: {
        return (m_cursor.getRow() != m_file.getNumberOfParagrahps() - 1); //TODO cover edge cases if needed
    }

    case Direction::LEFT: {
        return (m_cursor.getColumn() != 0 || m_cursor.getRow() != 0);
    }

    case Direction::RIGHT: {
        return (m_cursor.getRow() != m_file.getNumberOfParagrahps() - 1
            || static_cast<size_t>(m_cursor.getColumn())
            < m_file.getParagraph(m_file.getNumberOfParagrahps() - 1).length());
    }
    
    default: {
        break;
    }

    }
}

int EditorState::calculateVisualLineOfCursor(int screen_width) const {
    int visual_line_of_cursor = m_cursor.getColumn() / screen_width; 
    
    for (int logical_line_index = 0; logical_line_index < m_cursor.getRow(); logical_line_index++) {
        visual_line_of_cursor += m_file.visualLinesOfParagraph(logical_line_index, screen_width);
    }

    return visual_line_of_cursor;
}

Position EditorState::skipOffscreenLines(int offscreen_visual_lines, int screen_width) const {
    for (int logical_line_index = 0; logical_line_index < m_cursor.getRow(); logical_line_index++) {
        int line_length = m_file.getParagraphLength(logical_line_index);
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

void EditorState::insertCharacterAt(char character_to_add, Position position) {
    m_file.insertCharacterAt(character_to_add, position);
    m_cursor.moveRight();
}

std::optional<char> EditorState::readCharacterAt(Position position) {
    if (m_file.getNumberOfParagrahps() <= position.row
        || m_file.getParagraph(position.row).length() <= static_cast<size_t>(position.column)) {
        return std::nullopt;
    }

    return m_file.getParagraph(position.row).at(position.column);
}

std::optional<char> EditorState::readCharacterAtCursor() {
    return readCharacterAt(m_cursor.getPosition());
}

void EditorState::setCharacterAt(char character_to_set, Position position) {
    // TextFile does error handeling
    m_file.setCharacterAt(character_to_set, position);
}

void EditorState::deleteRange(Position start, Position end) {
    m_file.deleteRange(start, end);
}

void EditorState::splitAtCursor() {
    m_file.splitAt(m_cursor.getPosition());
    moveCursorRight();
}

void EditorState::joinLineToPrevious(int line) {
    m_file.joinToPrevious(line);
}

Position EditorState::getFirstVisibleChar(ScreenSize text_area_size) const {
    int visual_line_of_cursor = calculateVisualLineOfCursor(text_area_size.width);

    // too far up to place cursor in the middle
    if (visual_line_of_cursor <= text_area_size.height / 2) {
        return {0,0};
    }

    // skip some lines to keep cursor in the middle
    int offscreen_visual_lines = visual_line_of_cursor - (text_area_size.height / 2);
    
    return skipOffscreenLines(offscreen_visual_lines, text_area_size.width);
}

void EditorState::addTemporaryMessage(std::string message) {
    m_temporary_messages.push_back(message);
}

const std::vector<std::string>& EditorState::getTemporaryMessages() const {
    return m_temporary_messages;
}

void EditorState::clearTemporaryMessages() {
    m_temporary_messages.clear();
}