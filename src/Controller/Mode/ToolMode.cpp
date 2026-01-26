#include <ncurses.h>
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Action/NullAction.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"

std::pair<ModeType, std::shared_ptr<Action>> ToolMode::parseInput(int input, ScreenSize size) {
    switch (input) {
    case 'a':
        return {ModeType::TYPING_MODE, std::make_shared<NullAction>()};
    case 'h':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(size, Direction::BACKWARD)};
    case 'j':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(size, Direction::DOWN)};
    case 'k':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(size, Direction::UP)};
    case 'l':
        return {ModeType::TOOL_MODE, std::make_shared<CharwiseMoveAction>(size, Direction::FORWARD)};
    case 'y':
        return {ModeType::TOOL_MODE, std::make_shared<SaveAction>()};
    default:
        return {ModeType::TOOL_MODE, std::make_shared<NullAction>()};
    }

}