#include "../../../inc/Controller/Action/IndentAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

IndentAction::IndentAction(int row, int max_indent_width):
    m_row{row},
    m_max_indent_width{max_indent_width}
    {}

void IndentAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    int leading_spaces = state.getParagraph(m_row).find_first_not_of(' ');
    m_spaces_added = m_max_indent_width - (leading_spaces % m_max_indent_width);

    for (int _ = 0; _ < m_spaces_added; _++) {
        state.insertCharacterAt(' ', {m_row, 0});
    }
}

void IndentAction::undo(EditorState& state) {
    state.deleteRange(
        {m_row, 0},
        {m_row, m_spaces_added - 1}
    );
    
    for (int _ = 0; _ < m_spaces_added; _++) {
        state.moveCursorLeft();
    }
}

bool IndentAction::canBeUndone() const {
    return true;
}