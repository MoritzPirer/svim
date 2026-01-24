#include "../../../inc/Controller/Mode/TypeMode.hpp"
#include "../../../inc/Controller/Action/NullAction.hpp"

std::pair<ModeType, std::shared_ptr<Action>> TypeMode::parseInput(int input) {
    (void) input; //TEMP

    return {ModeType::TOOL_MODE, std::make_shared<NullAction>()};
}