#include "../../../inc/Controller/Action/SectionMoveAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

SectionMoveAction::SectionMoveAction(Position start, Position end, Direction direction):
    RangedAction{start, end},
    m_direction{direction}
    {}

void SectionMoveAction::apply(ExecutionContext& context) {
    if (m_direction == Direction::LEFT) {
        context.state.moveCursorTo(m_start);
    }
    else {
        context.state.moveCursorTo(m_end);
    }
}