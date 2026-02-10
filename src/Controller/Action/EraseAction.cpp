#include "../../../inc/Controller/Action/EraseAction.hpp"

EraseAction::EraseAction(int offset, bool allow_overhang_erase):
    m_offset{offset},
    m_allow_overhang_erase{allow_overhang_erase}
    {}

void EraseAction::applyTo(EditorState& state) {
    /*
    NOTE:
    if erasing a single char further away from the cursor or to the right of the cursor is needed
    this logic needs to be reworked!
    */

    Position cursor_position = state.getCursor().getPosition();
    Position erase_position = cursor_position; 
    
    if (m_offset < 0) {
        // move left if possible
        if (erase_position.column > 0) {
            erase_position.column--;
        }
        // otherwise move to end of prev if possible
        else if (erase_position.row > 0) {
            erase_position.row--;
            erase_position.column = state.getParagraph(erase_position.row).length() - 1;
        } 
        // if already at (0/0) erase offset is impossible
        else {
            return;
        }
        
    }
    else if (m_offset > 0) {
        return;
    }

    if (erase_position.row < cursor_position.row) {
        state.moveCursorLeft();
        state.joinLineToPrevious(cursor_position.row);
    }

    // erase position is overhang
    else if (static_cast<size_t>(erase_position.column) == state.getParagraph(erase_position.row).length()
        && static_cast<size_t>(cursor_position.row + 1) < state.getNumberOfParagrahps()) {

        if (m_allow_overhang_erase) {

            state.moveCursorLeft();
            state.joinLineToPrevious(cursor_position.row + 1);
            state.moveCursorRight();
        }
    }
    else {
        state.deleteRange(erase_position, erase_position);
        if (m_offset < 0) {
            state.moveCursorLeft();
        }
    }
}