#include "../../../inc/Controller/Action/DelimiterCaseSetAction.hpp"

DelimiterCaseSetAction::DelimiterCaseSetAction(
    std::string delimiters,
    std::string anti_delimiters,
    Direction direction,
    bool paragraph_is_delimiter,
    Case target_case
):
    DelimiterAction{delimiters, anti_delimiters, direction, EndBehavior::STOP_BEFORE_END, paragraph_is_delimiter},
    m_target_case{target_case}
    {}

int DelimiterCaseSetAction::getStartColumn(const EditorState& state, int row) {
    Position cursor_position = state.getCursor().getPosition();

    if (row == cursor_position.row) {
        return cursor_position.column;
    }

    if (m_move_direction == Direction::LEFT) {
        return state.getParagraph(row).length() - 1;
    }

    return 0;
}

int DelimiterCaseSetAction::getEndColumn(const EditorState& state, int row, Position stop_position) {
    if (row == stop_position.row) {
        return stop_position.column;
    }

    if (m_move_direction == Direction::LEFT) {
        return 0;
    }

    return state.getParagraph(row).length() - 1;
}

void DelimiterCaseSetAction::applyTo(EditorState& state) {
    Position stop_position = findStopPosition(state); 

    int step = m_move_direction == Direction::LEFT? -1 : 1;

    for (int row = state.getCursor().getRow(); row * step <= stop_position.row * step; row += step) {
        if (state.getParagraph(row).length() == 0) {
            continue;
        }

        int start_column = getStartColumn(state, row);
        int end_column = getEndColumn(state, row, stop_position);

        for (int column = std::min(start_column, end_column);
            column <= std::max(start_column, end_column); column++) {

            //protect from overhang cursor position
            if (static_cast<size_t>(column) >= state.getParagraph(row).length()) {
                continue;
            }
            
            char character = *state.readCharacterAt({row, column});

            character = (m_target_case == Case::UPPER_CASE)?
                std::toupper(character) : std::tolower(character);

            state.setCharacterAt(character, {row, column});
        }
    }
}

