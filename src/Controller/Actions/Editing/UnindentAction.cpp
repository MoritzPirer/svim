#include "../../../../inc/Controller/Actions/Editing/UnindentAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

UnindentAction::UnindentAction(int row, int indent_width):
    m_row{row},
    m_max_indent_width{indent_width}
    {}

void UnindentAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    setSpacesRemoved(state);
    deleteSpaces(state);
    adjustCursor(state);    

    state.requestBackup();
}

void UnindentAction::setSpacesRemoved(const EditorState& state) {
    size_t after_whitespace = state.getParagraph(m_row).find_first_not_of(' ');

    int leading_spaces = (after_whitespace == std::string::npos) ?
        state.getParagraph(m_row).length() :
        after_whitespace;

    m_spaces_removed = std::min(leading_spaces, m_max_indent_width);
}

void UnindentAction::deleteSpaces(EditorState& state) {
    if (m_spaces_removed > 0) {
        state.deleteRange(
            {m_row, 0},
            {m_row, m_spaces_removed - 1}
        );
    } 
}

void UnindentAction::adjustCursor(EditorState& state) {
    for (int _ = 0; _ < m_spaces_removed; _++) {
        state.moveCursorLeft();
    }
}

void UnindentAction::undo(EditorState& state) {
    for (int _ = 0; _ < m_spaces_removed; _++) {
        state.insertCharacterAt(' ', {m_row, 0});
    }

    state.requestBackup();
}

bool UnindentAction::canBeUndone() const {
    return true;
}