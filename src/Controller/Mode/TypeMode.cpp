#include "../../../inc/Controller/Mode/TypeMode.hpp"
#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Action/NullAction.hpp"

std::pair<ModeType, std::shared_ptr<Action>> TypeMode::parseInput(int input) {
    if (input == 27 /* == newline */) {
        return {ModeType::TOOL_MODE, std::make_shared<NullAction>()};
    }
    //MODO: HANDLE DELETE

    //special case for newline?
    return {ModeType::TYPE_MODE, std::make_shared<InsertAction>(input)};
}