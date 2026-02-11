#include "../../../inc/Controller/Action/IndentAction.hpp"

IndentAction::IndentAction(int indent_width):
    m_indent_width{indent_width}
    {}

void IndentAction::applyTo(EditorState& state) {
    Position insertion_position = {
        state.getCursor().getRow(),
        0
    };

    for (int _ = 0; _ < m_indent_width; _++) {
        state.insertCharacterAt(' ', insertion_position);
    }
}