#include <unordered_map>
#include <functional>

#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

CharwiseMoveAction::CharwiseMoveAction(ScreenSize size, Direction direction):
    m_size{size},
    m_direction{direction} {}

void CharwiseMoveAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;
    
    std::unordered_map<Direction, std::function<void(void)>> move_actions = {
        {Direction::RIGHT, [&]() -> void { state.moveCursorRight(); }},
        {Direction::LEFT, [&]() -> void { state.moveCursorLeft(); }},
        {Direction::UP, [&]() -> void { state.moveCursorUp(m_size.width); }},
        {Direction::DOWN, [&]() -> void { state.moveCursorDown(m_size.width); }},
    };

    if (move_actions.contains(m_direction)) {
        move_actions.at(m_direction)();
    }
}