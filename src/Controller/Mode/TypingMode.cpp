#include "../../../inc/Controller/Mode/TypingMode.hpp"
#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Action/NullAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"

#include "../../../inc/Controller/Control/SpecialInputs.hpp"

std::pair<ModeType, std::shared_ptr<Action>> TypingMode::parseInput(int input, ScreenSize size) {
    (void) size;
    
    switch (input) {
    case INPUT_ESCAPE: {
        return {ModeType::TOOL_MODE, std::make_shared<NullAction>()};
    }

    case INPUT_BACKSPACE: {
        return {ModeType::TYPING_MODE, std::make_shared<DeleteAction>(std::nullopt, std::nullopt)};
    }

    case INPUT_ENTER: {
        return {ModeType::TYPING_MODE, std::make_shared<ParagraphSplittingAction>()};
    }

    default: {
        return {ModeType::TYPING_MODE, std::make_shared<InsertAction>(input)};
    }
    }

    //special case for newline?
}