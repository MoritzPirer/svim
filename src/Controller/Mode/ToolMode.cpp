#include <ncurses.h>
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Action/NullAction.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"

std::pair<ModeType, std::shared_ptr<Action>>  ToolMode::parseInput(int input) {
    switch (input) {
    case 'a':
        return {ModeType::TYPING_MODE, std::make_shared<NullAction>()};
    case 'h':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(Direction::BACKWARD)};
    case 'j':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(Direction::DOWN)};
    case 'k':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(Direction::UP)};
    case 'l':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(Direction::FORWARD)};
    default:
        return {ModeType::TOOL_MODE, std::make_shared<NullAction>()};
    }

}