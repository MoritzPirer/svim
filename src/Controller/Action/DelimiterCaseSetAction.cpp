#include "../../../inc/Controller/Action/DelimiterCaseSetAction.hpp"

DelimiterCaseSetAction::DelimiterCaseSetAction(
    ScreenSize size,
    std::string delimiters,
    Direction direction,
    Case target_case
):
    DelimiterAction{size, delimiters, direction, EndBehavior::STOP_BEFORE_END, false},
    m_target_case{target_case}
    {}

void DelimiterCaseSetAction::applyTo(EditorState& state) {
    Position cursor_position = state.getCursor().getPosition();

    //find ends of range
    Position stop_position = findStopPosition(state); 

    int step = m_move_direction == Direction::LEFT? -1 : 1;

    for (int row = cursor_position.row; row * step <= stop_position.row * step; row += step) {
        if (state.getParagraph(row).length() == 0) {
            continue;
        }

        int start_column = (row == cursor_position.row? cursor_position.column : 0);
        int end_column = (row == stop_position.row? 
            stop_position.column : std::max(state.getParagraph(row).length() - 1, 0ul));
        
        for (int column = std::min(start_column, end_column); column <= std::max(start_column, end_column); column++) {
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