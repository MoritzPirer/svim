#include "../../../inc/Controller/Action/DelimiterMoveAction.hpp"

DelimiterMoveAction::DelimiterMoveAction(
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

void DelimiterMoveAction::applyTo(EditorState& state) {
    bool has_reached_delimiter = false;

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
            bool cross_into_next_paragraph = true;
            if (cross_into_next_paragraph) {
                continue;
            }

            state.moveCursor(getOppositeDirection(m_move_direction), m_size.width);
            break;
        }

        // stop at first delimiter after moving at least once
        if (m_delimiters.find(*character) != std::string::npos) {
            has_reached_delimiter = true;
            
            if (m_end_behavior == EndBehavior::STOP_BEFORE_END) {
                state.moveCursor(getOppositeDirection(m_move_direction), m_size.width);
                break;
            }

            if (m_end_behavior == EndBehavior::STOP_ON_END) {
                break;
            }
        }
        else if (has_reached_delimiter && m_end_behavior == EndBehavior::STOP_AFTER_END) {
           break; 
        }
    }
}