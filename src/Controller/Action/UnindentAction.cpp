#include "../../../inc/Controller/Action/UnindentAction.hpp"

UnindentAction::UnindentAction(int indent_width):
    m_indent_width{indent_width}
    {}

void UnindentAction::applyTo(EditorState& state) {
    Position erase_position = {
        state.getCursor().getRow(),
        0
    };

    
    for (int _ = 0; _ < m_indent_width; _++) {
        std::optional<char> character = state.readCharacterAt(erase_position);

        if (!character.has_value() || *character != ' ') {
            break;
        }

        state.deleteRange(erase_position, erase_position);
        state.moveCursorLeft();
    }
}