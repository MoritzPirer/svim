#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"

CharwiseMoveAction::CharwiseMoveAction(ScreenSize size, Direction direction):
    SizeDependantAction{size},
    m_direction{direction} {}

void CharwiseMoveAction::applyTo(EditorState& state) {
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
        state.moveCursorUp(m_size.width);
        break;
    }
    case Direction::DOWN: {
        state.moveCursorDown(m_size.width);
        break;
    }
    default:
        throw std::invalid_argument("Unknown enum value!");
    }
}