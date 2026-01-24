#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"

CharwiseMoveAction::CharwiseMoveAction(Direction direction):
    m_direction{direction} {}

void CharwiseMoveAction::applyTo(EditorState& state, ScreenSize size) {
    switch (m_direction) {
    case Direction::FORWARD: {
        state.moveCursorRight();
        break;
    }
    case Direction::BACKWARD: {
        state.moveCursorLeft();
        break;
    }
    case Direction::UP: {
        state.moveCursorUp(size.width);
        break;
    }
    case Direction::DOWN: {
        state.moveCursorDown(size.width);
        break;
    }
    default:
        throw std::invalid_argument("Unknown enum value!");
    }
}