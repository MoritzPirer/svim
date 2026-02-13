#include "../../../inc/Controller/Action/DelimiterMoveAction.hpp"

DelimiterMoveAction::DelimiterMoveAction(
    std::string delimiters,
    std::string anti_delimiters,
    Direction move_direction,
    EndBehavior end_behavior,
    bool paragraph_is_delimiter
):
    DelimiterAction{delimiters, anti_delimiters, move_direction, end_behavior, paragraph_is_delimiter}
    {}

void DelimiterMoveAction::applyTo(EditorState& state) {
    state.moveCursorTo(findStopPosition(state));
}