#include "../../../inc/Controller/Mode/TypingMode.hpp"
#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"

#include "../../../inc/Shared/SpecialInputs.hpp"

std::pair<ModeType, std::vector<std::shared_ptr<Action>>> TypingMode::parseInput(
    int input, ScreenSize size, Settings settings) {

    (void) settings;
    
    switch (input) {
        case ARROW_LEFT:
            return {ModeType::TYPING_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::BACKWARD)}};
        case ARROW_DOWN:
            return {ModeType::TYPING_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::DOWN)}};
        case ARROW_UP:
            return {ModeType::TYPING_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::UP)}};
        case ARROW_RIGHT:
            return {ModeType::TYPING_MODE, {std::make_shared<CharwiseMoveAction>(size, Direction::FORWARD)}};
            
        case INPUT_ESCAPE: {
            return {ModeType::TOOL_MODE, {}};
        }

        case INPUT_BACKSPACE: {
            return {ModeType::TYPING_MODE, {std::make_shared<EraseAction>(-1)}};
        }

        case INPUT_ENTER: {
            return {ModeType::TYPING_MODE, {std::make_shared<ParagraphSplittingAction>()}};
        }

        default: {
            return {ModeType::TYPING_MODE, {std::make_shared<InsertAction>(input)}};
        }
    }

    //special case for newline?
}