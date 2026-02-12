#include "../../../inc/Controller/Action/EraseAction.hpp"

EraseAction::EraseAction(int offset, bool allow_overhang_erase):
    m_offset{offset},
    m_allow_overhang_erase{allow_overhang_erase}
    {}

std::optional<Position> EraseAction::findErasePosition(const EditorState& state) {
    Position erase_position = state.getCursor().getPosition();
    
    if (m_offset < 0) {
        if (erase_position.column > 0) { // move left if possible
            erase_position.column--;
            return erase_position;
        }
        else if (erase_position.row > 0) { // otherwise move to end of prev if possible
            erase_position.row--;
            erase_position.column = state.getParagraph(erase_position.row).length() - 1;
            return erase_position;
        } 

        return std::nullopt;
    }
    else if (m_offset > 0) {
        throw std::invalid_argument("positive offset not yet supported!");
    }
    else {
        return erase_position;
    }
}

void EraseAction::applyTo(EditorState& state) {
    /*
    NOTE:
    if erasing a single char further away from the cursor or to the right of the cursor is needed
    this logic needs to be reworked!
    */

    Position cursor_position = state.getCursor().getPosition();
    std::optional<Position> erase_position = findErasePosition(state); 
    if (!erase_position.has_value()) {
        return;
    }

    if (erase_position->row < cursor_position.row) {
        state.moveCursorLeft();
        state.joinLineToPrevious(cursor_position.row);
    }

    // erase position is overhang
    else if (static_cast<size_t>(erase_position->column) == state.getParagraph(erase_position->row).length()
        && static_cast<size_t>(cursor_position.row + 1) < state.getNumberOfParagrahps()) {

        if (m_allow_overhang_erase) {

            state.moveCursorLeft();
            state.joinLineToPrevious(cursor_position.row + 1);
            state.moveCursorRight();
        }
    }
    else {
        state.deleteRange(*erase_position, *erase_position);
        if (m_offset < 0) {
            state.moveCursorLeft();
        }
    }
}