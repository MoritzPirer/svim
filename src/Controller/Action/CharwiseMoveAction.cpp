#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

CharwiseMoveAction::CharwiseMoveAction(ScreenSize size, Direction direction):
    m_size{size},
    m_direction{direction} {}

void CharwiseMoveAction::apply(ExecutionContext& context) {
    switch (m_direction) {
    case Direction::RIGHT: {
        context.state.moveCursorRight();
        break;
    }

    case Direction::LEFT: {
        context.state.moveCursorLeft();
        break;
    }

    case Direction::UP: {
        context.state.moveCursorUp(m_size.width);
        break;
    }

    case Direction::DOWN: {
        context.state.moveCursorDown(m_size.width);
        break;
    }

    default: {
        throw std::invalid_argument("Unknown enum value!");
    }

    }
}