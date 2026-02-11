#include "../../../inc/Controller/Action/DelimiterAction.hpp"

DelimiterAction::DelimiterAction(
    ScreenSize size,
    std::string delimiters,
    Direction move_direction,
    EndBehavior end_behavior,
    bool paragraph_is_delimiter
):
    m_size{size},
    m_delimiters{delimiters},
    m_move_direction{move_direction},
    m_end_behavior{end_behavior},
    m_paragraph_is_delimiter{paragraph_is_delimiter}
    {}

Position DelimiterAction::findStopPosition(EditorState& state) {
    Position original_cursor_position = state.getCursor().getPosition();

    bool has_reached_delimiter = false;
    bool has_reached_non_delimiter = false;

    while (state.canCursorMove(m_move_direction)) {
        int row_before = state.getCursor().getRow();
        state.moveCursor(m_move_direction, m_size.width);
        int row_after = state.getCursor().getRow();

        std::optional<char> character = state.readCharacterAtCursor();

        if (m_paragraph_is_delimiter && row_before != row_after) {
            if (m_end_behavior == EndBehavior::STOP_BEFORE_END) {
                state.moveCursor(getOppositeDirection(m_move_direction), m_size.width);
                break;
            }
            has_reached_delimiter = true;
        }

        // handle overhang cursor position
        if (!character.has_value()){
            continue;
        }

        // stop at first delimiter after moving at least once
        if (m_delimiters.find(*character) == std::string::npos) {
            has_reached_non_delimiter = true;
            if (has_reached_delimiter && m_end_behavior == EndBehavior::STOP_AFTER_END) {
                break; 
            }
        }
        else {
            has_reached_delimiter = true;
            
            if (has_reached_non_delimiter && m_end_behavior == EndBehavior::STOP_BEFORE_END) {
                state.moveCursor(getOppositeDirection(m_move_direction), m_size.width);
                break;
            }

            if (m_end_behavior == EndBehavior::STOP_ON_END) {
                break;
            }
        }
    }

    Position stop_position = state.getCursor().getPosition();
    state.moveCursorTo(original_cursor_position);

    return stop_position;
}
