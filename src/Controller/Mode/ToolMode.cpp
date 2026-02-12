#include <ncurses.h>
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Action/IndentAction.hpp"
#include "../../../inc/Controller/Action/UnindentAction.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
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

ParseResult ToolMode::parseSpecialKey(SpecialKey key, ScreenSize text_area_size, const Settings& settings) {
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

    case SpecialKey::TAB: {
        return {ModeType::TOOL_MODE, {
            make_shared<IndentAction>(settings.isEnabled("do_skinny_tabs")? 2 : 4)
        }};
    }

    case SpecialKey::SHIFT_TAB: {
        return {ModeType::TOOL_MODE, {
            std::make_shared<UnindentAction>(settings.isEnabled("do_skinny_tabs")? 2 : 4)
        }};
    }

    default: {
        return {ModeType::TOOL_MODE, {}};
    }

    }
}

ParseResult ToolMode::parseStandardInput(int input, ScreenSize text_area_size, const Settings& settings) {
    return m_command_parser.parseInput(input, text_area_size, settings);
}

ParseResult ToolMode::parseInput(
    Input input, ScreenSize actual_size, ScreenSize text_area_size, const Settings& settings) {
    
    if (input.mouse_position.has_value()) {
        return parseMouseMovement(*input.mouse_position, actual_size, text_area_size);
    }

    if (input.special_key.has_value()) {
        return parseSpecialKey(*input.special_key, text_area_size, settings);
    }

    if (input.standard_input.has_value()) {
        return parseStandardInput(*input.standard_input, text_area_size, settings);
    }
    
    return {ModeType::TOOL_MODE, {}};
}