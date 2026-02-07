#include <ncurses.h>
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"

#include "../../../inc/Controller/Settings/Settings.hpp"
#include "../../../inc/Shared/SpecialKey.hpp"

using std::make_shared;

ParseResult ToolMode::parseMouseMovement(Position click_position,
    ScreenSize actual_size, ScreenSize text_area_size) {

    /// validate click position
    int aside_width = actual_size.width - text_area_size.width;

    if (click_position.column < aside_width || click_position.column > actual_size.width) {
        return {ModeType::TOOL_MODE, {}};
    }
    if (click_position.row < 0 || click_position.row > text_area_size.height) {
        return {ModeType::TOOL_MODE, {}};
    }

    Position adjusted_position = {
        click_position.row,
        click_position.column - aside_width
    };

    return {ModeType::TOOL_MODE, {make_shared<FixedPositionMoveAction>(text_area_size, adjusted_position)}};
}

ParseResult ToolMode::parseSpecialKey(SpecialKey key, ScreenSize text_area_size) {
    switch (key) {
        case SpecialKey::ARROW_LEFT: {
            return {ModeType::TOOL_MODE, {
                make_shared<CharwiseMoveAction>(text_area_size, Direction::LEFT)
            }};
        }
        case SpecialKey::ARROW_DOWN: {
            return {ModeType::TOOL_MODE, {
                make_shared<CharwiseMoveAction>(text_area_size, Direction::DOWN)
            }};
        }
        case SpecialKey::ARROW_UP: {
            return {ModeType::TOOL_MODE, {
                make_shared<CharwiseMoveAction>(text_area_size, Direction::UP)
            }};
        }
        case SpecialKey::ARROW_RIGHT: {
            return {ModeType::TOOL_MODE, {
                make_shared<CharwiseMoveAction>(text_area_size, Direction::RIGHT)
            }};
        }
        default:
            return {ModeType::TOOL_MODE, {}};
    }
}

ParseResult ToolMode::parseStandardInput(int input, ScreenSize text_area_size, const Settings& settings) {
    return m_command_parser.parseInput(input, text_area_size, settings);
    /*
    switch (input) {
        // move actions
        case 'h':
        return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(text_area_size, Direction::BACKWARD)}};
        case 'j':
        return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(text_area_size, Direction::DOWN)}};
        case 'k':
        return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(text_area_size, Direction::UP)}};
        case 'l':
        return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(text_area_size, Direction::FORWARD)}};
        case 'g':
        return {ModeType::TOOL_MODE, {make_shared<ChunkwiseMoveAction>(Scope::FILE, Destination::START)}};
        case 'G':
        return {ModeType::TOOL_MODE, {make_shared<ChunkwiseMoveAction>(Scope::FILE, Destination::END)}};
        case '0':
        return {ModeType::TOOL_MODE, {make_shared<ChunkwiseMoveAction>(Scope::PARAGRAPH, Destination::START)}};
        case '$':
        return {ModeType::TOOL_MODE, {make_shared<ChunkwiseMoveAction>(Scope::PARAGRAPH, Destination::END)}};
        
        case 'w':
        return {ModeType::TOOL_MODE, {
            make_shared<DirectionalMoveAction>(Scope::WORD, Destination::START, text_area_size, Direction::FORWARD)
        }};
        case 'W':
        return {ModeType::TOOL_MODE, {
            make_shared<DirectionalMoveAction>(Scope::EXPRESSION, Destination::START, text_area_size, Direction::FORWARD)
        }};
        case 'b':
        return {ModeType::TOOL_MODE, {
            make_shared<DirectionalMoveAction>(Scope::WORD, Destination::START, text_area_size, Direction::BACKWARD)
        }};
        case 'B':
        return {ModeType::TOOL_MODE, {
            make_shared<DirectionalMoveAction>(Scope::EXPRESSION, Destination::START, text_area_size, Direction::BACKWARD)
        }};
        case 'e':
        return {ModeType::TOOL_MODE, {
            make_shared<DirectionalMoveAction>(Scope::WORD, Destination::END, text_area_size, Direction::FORWARD)
        }};
        case 'E':
        return {ModeType::TOOL_MODE, {
            make_shared<DirectionalMoveAction>(Scope::EXPRESSION, Destination::END, text_area_size, Direction::FORWARD)
        }};
        
        // mode switching actions
        case 'i':
        return {ModeType::TYPING_MODE, {}};
        case 'a':
        return {ModeType::TYPING_MODE, {make_shared<CharwiseMoveAction>(text_area_size, Direction::FORWARD)}};
        case 'I':
        return {ModeType::TYPING_MODE, {make_shared<ChunkwiseMoveAction>(Scope::PARAGRAPH, Destination::START)}};
        case 'A':
        return {ModeType::TYPING_MODE, {make_shared<ChunkwiseMoveAction>(Scope::PARAGRAPH, Destination::END)}};
        case 'o':
        return {ModeType::TYPING_MODE, {
            make_shared<ChunkwiseMoveAction>(Scope::PARAGRAPH, Destination::END),
            make_shared<ParagraphSplittingAction>()
        }};
        case 'O':
        return {ModeType::TYPING_MODE, {
            make_shared<ChunkwiseMoveAction>(Scope::PARAGRAPH, Destination::START),
            make_shared<ParagraphSplittingAction>(),
            make_shared<CharwiseMoveAction>(text_area_size, Direction::UP)
        }};
        
        // file actions
        case 't': // temporary shortcut
        return {ModeType::TOOL_MODE, {
            make_shared<SaveAction>(settings.isEnabled("confirm_save"))
        }};
        case 'q':
        return {ModeType::TOOL_MODE, {make_shared<QuitAction>(false)}};
        case 'Q': // temporary, force quit
        return {ModeType::TOOL_MODE, {make_shared<QuitAction>(true)}};
        case 'T': // temporary, save& quit
        return {ModeType::TOOL_MODE, {
            make_shared<SaveAction>(settings.isEnabled("confirm_save")),
            make_shared<QuitAction>(true)
        }};
        
        // deletion actions
        case 'x':
        return {ModeType::TOOL_MODE, {make_shared<EraseAction>(0)}};
        case 's':
        return {ModeType::TYPING_MODE, {make_shared<EraseAction>(0)}};
        
        default:
        return {ModeType::TOOL_MODE, {}};
    }
    */
}

ParseResult ToolMode::parseInput(
    Input input, ScreenSize actual_size, ScreenSize text_area_size, const Settings& settings) {
    
    if (input.mouse_position.has_value()) {
        return parseMouseMovement(*input.mouse_position, actual_size, text_area_size);
    }

    if (input.special_key.has_value()) {
        return parseSpecialKey(*input.special_key, text_area_size);
    }

    if (input.standard_input.has_value()) {
        return parseStandardInput(*input.standard_input, text_area_size, settings);
    }
    
    return {ModeType::TOOL_MODE, {}};
}