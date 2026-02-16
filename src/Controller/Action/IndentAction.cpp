#include "../../../inc/Controller/Action/IndentAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

IndentAction::IndentAction(int indent_width):
    m_indent_width{indent_width}
    {}

void IndentAction::apply(ExecutionContext& context) {
    Position insertion_position = {
        context.state.getCursor().getRow(),
        0
    };

    for (int _ = 0; _ < m_indent_width; _++) {
        context.state.insertCharacterAt(' ', insertion_position);
    }
}