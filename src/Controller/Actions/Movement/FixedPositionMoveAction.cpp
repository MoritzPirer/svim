#include "../../../../inc/Controller/Actions/Movement/FixedPositionMoveAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

FixedPositionMoveAction::FixedPositionMoveAction(Position target_position):
    m_target_position{target_position} {}

void FixedPositionMoveAction::apply(ExecutionContext& context) {
    //TODO WHEN implementing bookmarks
    // Most of this logic is for converting mouse click position to logical position
    // extract that and move it to a different spot

    context.state.moveCursorTo(m_target_position); 
}