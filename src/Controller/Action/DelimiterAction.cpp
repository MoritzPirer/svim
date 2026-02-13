#include "../../../inc/Controller/Action/DelimiterAction.hpp"

DelimiterAction::DelimiterAction(
    std::string delimiters,
    std::string anti_delimiters,
    Direction move_direction,
    EndBehavior end_behavior,
    bool paragraph_is_delimiter
):
    m_delimiters{delimiters},
    m_anti_delimiters{anti_delimiters},
    m_move_direction{move_direction},
    m_end_behavior{end_behavior},
    m_paragraph_is_delimiter{paragraph_is_delimiter}
    {}

bool DelimiterAction::isDelimiter(char c) {
    return (m_delimiters.find(c) != std::string::npos);
}

bool DelimiterAction::isAntiDelimiter(char c) {
    return (m_anti_delimiters.find(c) != std::string::npos);
}

bool DelimiterAction::isSuitableEndpoint(EditorState& state, char character, 
    bool& has_reached_delimiter, bool& has_reached_non_delimiter, int& delimiter_balance) {
    
    if (isDelimiter(character)) {
        has_reached_delimiter = true;
        
        if (delimiter_balance != 0) {
            delimiter_balance--;
            return false;
        }
        
        if (m_end_behavior == EndBehavior::STOP_BEFORE_END && has_reached_non_delimiter) {
            //move back off of delimiter
            state.moveCursorSideways(getOppositeDirection(m_move_direction));
            return true;
        }

        if (m_end_behavior == EndBehavior::STOP_ON_END) {
            return true;
        }
        
        if (!m_anti_delimiters.empty()) {
            delimiter_balance--;
        }
    }
    else {
        has_reached_non_delimiter = true;

        if (m_end_behavior == EndBehavior::STOP_AFTER_END && has_reached_delimiter
            && delimiter_balance == 0) {
            return true; 
        }
        
        if (isAntiDelimiter(character)) {
            delimiter_balance++;
        }
    }

    return false;
}

Position DelimiterAction::findStopPosition(EditorState& state) {
    Position original_cursor_position = state.getCursor().getPosition();

    bool has_reached_delimiter = false;
    
    std::optional<char> character = state.readCharacterAtCursor();
    bool has_reached_non_delimiter = (!character.has_value() || !isDelimiter(*character));

    int delimiter_balance = 0;
    bool search_is_over = false;
    while (state.canCursorMove(m_move_direction) && !search_is_over) {

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

        search_is_over = isSuitableEndpoint(state, *character, has_reached_delimiter,
            has_reached_non_delimiter, delimiter_balance);
    }

    // restore cursor position
    Position stop_position = state.getCursor().getPosition();
    state.moveCursorTo(original_cursor_position);

    return stop_position;
}
