#include "../../../inc/Controller/Mode/TypingMode.hpp"
#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"

#include "../../../inc/Shared/SpecialKey.hpp"

using std::make_shared;

ParseResult TypingMode::parseMouseMovement(Position click_position,
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

    return {ModeType::TYPING_MODE, {make_shared<FixedPositionMoveAction>(text_area_size, adjusted_position)}};}

ParseResult TypingMode::parseSpecialKey(SpecialKey key, ScreenSize text_area_size) {
    switch (key) {
    case SpecialKey::ARROW_LEFT: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(text_area_size, Direction::LEFT)
        }};
    }

    case SpecialKey::ARROW_DOWN: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(text_area_size, Direction::DOWN)
        }};
    }

    case SpecialKey::ARROW_UP: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(text_area_size, Direction::UP)
        }};
    }

    case SpecialKey::ARROW_RIGHT: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(text_area_size, Direction::RIGHT)
        }};
    }

    case SpecialKey::ESCAPE: {
        return {ModeType::TOOL_MODE, {}};
    }

    case SpecialKey::BACKSPACE: {
        return {ModeType::TYPING_MODE, {std::make_shared<EraseAction>(-1)}};
    }

    case SpecialKey::ENTER: {
        return {ModeType::TYPING_MODE, {std::make_shared<ParagraphSplittingAction>()}};
    }

    default: {
        return {ModeType::TYPING_MODE, {}};
    }

    }
}

ParseResult TypingMode::parseInput(
    Input input, ScreenSize actual_size, ScreenSize text_area_size, const Settings& settings) {

    (void) settings;
       
    if (input.mouse_position.has_value()) {
        return parseMouseMovement(*input.mouse_position, actual_size, text_area_size);
    }

    if (input.special_key.has_value()) {
        return parseSpecialKey(*input.special_key, text_area_size);
    }

    if (input.standard_input.has_value()) {
        return {ModeType::TYPING_MODE, {std::make_shared<InsertAction>(*input.standard_input)}};
    } 
    
    return {ModeType::TYPING_MODE, {}};
}