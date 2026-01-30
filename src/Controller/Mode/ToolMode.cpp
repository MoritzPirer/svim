#include <ncurses.h>
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"

std::pair<ModeType, std::vector<std::shared_ptr<Action>>> ToolMode::parseInput(int input, ScreenSize size) {
    switch (input) {
        case 'a':
            return {ModeType::TYPING_MODE, {}};
        case 'h':
            return {ModeType::TOOL_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::BACKWARD)}};
        case 'j':
            return {ModeType::TOOL_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::DOWN)}};
        case 'k':
            return {ModeType::TOOL_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::UP)}};
        case 'l':
            return {ModeType::TOOL_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::FORWARD)}};
        case 't': // temporary shortcut
            return {ModeType::TOOL_MODE, {std::make_shared<SaveAction>()}};
        case 'q':
            return {ModeType::TOOL_MODE, {std::make_shared<QuitAction>()}};
        case 'x':
            return {ModeType::TOOL_MODE, {std::make_shared<EraseAction>(0)}};
        case 's':
            return {ModeType::TYPING_MODE, {std::make_shared<EraseAction>(0)}};
        default:
            return {ModeType::TOOL_MODE, {}};
        }

}