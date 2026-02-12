#include "../../../inc/Controller/Action/DelimiterCaseSetAction.hpp"

DelimiterCaseSetAction::DelimiterCaseSetAction(
    std::string delimiters,
    Direction direction,
    bool paragraph_is_delimiter,
    Case target_case
):
    DelimiterAction{delimiters, direction, EndBehavior::STOP_BEFORE_END, paragraph_is_delimiter},
    m_target_case{target_case}
    {}

void DelimiterCaseSetAction::applyTo(EditorState& state) {
    Position cursor_position = state.getCursor().getPosition();
    Position stop_position = findStopPosition(state); 

    int step = m_move_direction == Direction::LEFT? -1 : 1;

    for (int row = cursor_position.row; row * step <= stop_position.row * step; row += step) {
        if (state.getParagraph(row).length() == 0) {
            continue;
        }

        int start_column;
        if (row == cursor_position.row) {
            start_column = cursor_position.column;
        }
        else {
            start_column = (m_move_direction == Direction::LEFT? state.getParagraph(row).length() - 1 : 0);
        }

        int end_column;
        if (row == stop_position.row) {
            end_column = stop_position.column;
        }
        else {
            end_column = (m_move_direction == Direction::LEFT? 0 : state.getParagraph(row).length() - 1);
        }

        for (int column = std::min(start_column, end_column);
            column <= std::max(start_column, end_column); column++) {
            //protect from overhang cursor position
            if (static_cast<size_t>(column) >= state.getParagraph(row).length()) {
                continue;
            }

            setCaseAt(state, {row, column});
        }
    }
}

void DelimiterCaseSetAction::setCaseAt(EditorState& state, Position position) {
    // if this throws bad_optional_access, there is a bug somewhere
    char character = *state.readCharacterAt(position);

    switch (m_target_case) {
    case Case::UPPER_CASE: {
        character = std::toupper(character);
        break;
    }

    case Case::LOWER_CASE: {
        character = std::tolower(character);
        break;
    }
    }

    state.setCharacterAt(character, position);

}