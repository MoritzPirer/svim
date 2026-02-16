#include "../../../inc/Controller/Action/ScopeMoveAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

ScopeMoveAction::ScopeMoveAction(
    ScreenSize size,
    EndBehavior end_behavior,
    Scope scope,
    Direction move_direction
):
    ScopeAction(size, end_behavior),
    m_scope{scope},
    m_move_direction{move_direction}
    {}

void ScopeMoveAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    switch (m_move_direction) {
    case Direction::LEFT: {
        state.moveCursorTo(startOfScope(state, m_scope));
        return;
    }

    case Direction::RIGHT: {
        state.moveCursorTo(endOfScope(state, m_scope));
        return;
    }

    default: {
        return;
    }
    }
}
