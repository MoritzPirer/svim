#include <memory>

#include "../../../inc/Controller/Mode/EditorMode.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"

bool EditorMode::isClickPositionValid(Position click_position,
    int aside_width, ScreenSize text_area_size) {

    // out of bounds horizontally
    if (click_position.column < aside_width
        || click_position.column > text_area_size.width + aside_width) {

        return false;
    }

    // out of bounds vertically
    if (click_position.row < 0 || click_position.row > text_area_size.height) {
        return false;
    }

    return true;
}

ParseResult EditorMode::parseMouseMovement(Position click_position,
    ScreenSize actual_size, ScreenSize text_area_size) {

    int aside_width = actual_size.width - text_area_size.width;

    if (!isClickPositionValid(click_position, aside_width, text_area_size)) {
        return {ModeType::TOOL_MODE, {}};
    }

    Position adjusted_position = {
        click_position.row,
        click_position.column - aside_width
    };

    return {ModeType::TYPING_MODE,
        std::make_shared<FixedPositionMoveAction>(text_area_size, adjusted_position)};
}