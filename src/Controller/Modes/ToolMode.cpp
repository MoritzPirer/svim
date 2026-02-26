#include "../../../inc/Controller/Modes/ToolMode.hpp"
#include "../../../inc/Controller/Parsing/ParsingContext.hpp"
#include "../../../inc/Controller/Actions/Editing/IndentAction.hpp"
#include "../../../inc/Controller/Actions/Editing/UnindentAction.hpp"
#include "../../../inc/Controller/Actions/Movement/CharwiseMoveAction.hpp"

#include "../../../inc/Controller/Services/Settings/Settings.hpp"
#include "../../../inc/Shared/Types/SpecialKey.hpp"

using std::make_shared;

ParseResult ToolMode::parseSpecialKey(SpecialKey key, ParsingContext context) {
    switch (key) {
    case SpecialKey::ARROW_LEFT: {
        return {ModeType::TOOL_MODE, 
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::LEFT)
        };
    }

    case SpecialKey::ARROW_DOWN: {
        return {ModeType::TOOL_MODE, 
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::DOWN)
        };
    }

    case SpecialKey::ARROW_UP: {
        return {ModeType::TOOL_MODE, 
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::UP)
        };
    }

    case SpecialKey::ARROW_RIGHT: {
        return {ModeType::TOOL_MODE, 
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::RIGHT)
        };
    }

    case SpecialKey::TAB: {
        return {ModeType::TOOL_MODE, 
            make_shared<IndentAction>(context.state.getCursor().getRow(), context.settings.getTabWidth())
        };
    }

    case SpecialKey::SHIFT_TAB: {
        return {ModeType::TOOL_MODE, 
            make_shared<UnindentAction>(context.state.getCursor().getRow(), context.settings.getTabWidth())
        };
    }

    default: {
        return {ModeType::TOOL_MODE, std::nullopt};
    }

    }
}

ParseResult ToolMode::parseStandardInput(int input, ParsingContext context) {
    return m_command_parser.parseInput(input, context);
}

ParseResult ToolMode::parseInput(Input input, ParsingContext context) {
    
    if (input.mouse_position.has_value()) {
        return parseMouseMovement(*input.mouse_position, context);
    }

    if (input.special_key.has_value()) {
        return parseSpecialKey(*input.special_key, context);
    }

    if (input.standard_input.has_value()) {
        return parseStandardInput(*input.standard_input, context);
    }
    
    return {ModeType::TOOL_MODE, std::nullopt};
}