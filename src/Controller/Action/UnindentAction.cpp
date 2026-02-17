#include "../../../inc/Controller/Action/UnindentAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

UnindentAction::UnindentAction(int row, int indent_width):
    m_row{row},
    m_max_indent_width{indent_width}
    {}

void UnindentAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    int leading_spaces = state.getParagraph(m_row).find_first_not_of(' ');
    m_spaces_removed = leading_spaces % m_max_indent_width;

    state.deleteRange(
        {m_row, 0},
        {m_row, m_spaces_removed - 1}
    );
    
    for (int _ = 0; _ < m_spaces_removed; _++) {
        state.moveCursorLeft();
    }
}

void UnindentAction::undo(EditorState& state) {
    for (int _ = 0; _ < m_spaces_removed; _++) {
        state.insertCharacterAt(' ', {m_row, 0});
    }
}

bool UnindentAction::canBeUndone() const {
    return true;
}