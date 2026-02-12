#include "../../../inc/Controller/Action/DelimiterAction.hpp"

DelimiterAction::DelimiterAction(
    std::string delimiters,
    Direction move_direction,
    EndBehavior end_behavior,
    bool paragraph_is_delimiter
):
    m_delimiters{delimiters},
    m_move_direction{move_direction},
    m_end_behavior{end_behavior},
    m_paragraph_is_delimiter{paragraph_is_delimiter}
    {}

bool DelimiterAction::isDelimiter(char c) {
    return (m_delimiters.find(c) != std::string::npos);
}

Position DelimiterAction::findStopPosition(EditorState& state) {
    Position original_cursor_position = state.getCursor().getPosition();

    bool has_reached_delimiter = false;
    
    std::optional<char> character = state.readCharacterAtCursor();
    bool has_reached_non_delimiter = (!character.has_value() || m_delimiters.find(*character) == std::string::npos);

    while (state.canCursorMove(m_move_direction)) {

        int row_before = state.getCursor().getRow();
        state.moveCursorSideways(m_move_direction);
        int row_after = state.getCursor().getRow();

        character = state.readCharacterAtCursor();

        if (m_paragraph_is_delimiter && row_before != row_after) {
            if (m_end_behavior == EndBehavior::STOP_BEFORE_END) {
                state.moveCursorSideways(getOppositeDirection(m_move_direction));
                break;
            }

            has_reached_delimiter = true;
        }

        // handle overhang cursor position
        if (!character.has_value()){
            continue;
        }

        if (isDelimiter(*character)) {
            has_reached_delimiter = true;
            
            if (m_end_behavior == EndBehavior::STOP_BEFORE_END && has_reached_non_delimiter) {
                //move back of delimiter
                state.moveCursorSideways(getOppositeDirection(m_move_direction));
                break;
            }

            if (m_end_behavior == EndBehavior::STOP_ON_END) {
                break;
            }
        }
        else {
            has_reached_non_delimiter = true;

            if (m_end_behavior == EndBehavior::STOP_AFTER_END && has_reached_delimiter) {
                break; 
            }
        }
    }

    // restore cursor position
    Position stop_position = state.getCursor().getPosition();
    state.moveCursorTo(original_cursor_position);

    return stop_position;
}
