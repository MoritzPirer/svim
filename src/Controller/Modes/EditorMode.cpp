#include <memory>

#include "../../../inc/Controller/Modes/EditorMode.hpp"
#include "../../../inc/Controller/Actions/Movement/FixedPositionMoveAction.hpp"

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

Position EditorMode::convertToLogicPosition(Position click_position, ParsingContext context) {
    EditorState& state = context.state;

    Position first_visible = state.getFirstVisibleChar(context.text_area_size);

    // how many visual lines are left in the first visible paragraph
    int lines_of_partial = TextFile::visualLinesNeeded(
        state.getParagraph(first_visible.row).length() - first_visible.column,
        context.text_area_size.width
    );

    int current_paragraph = first_visible.row;
    int visual_lines_consumed = 0;

    // handle first paragraph
    if (click_position.row < lines_of_partial) {
        int column = first_visible.column + click_position.column
            + click_position.row * context.text_area_size.width; 

        column = std::min<int>( column, state.getParagraph(current_paragraph).length());

        return {current_paragraph, column};
    }

    visual_lines_consumed += lines_of_partial;
    current_paragraph++;

    // full paragraphs
    while (static_cast<size_t>(current_paragraph) < state.getNumberOfParagrahps()) {
        int visual_lines_of_paragraph = TextFile::visualLinesNeeded(
            state.getParagraph(current_paragraph).length(),
            context.text_area_size.width
        );

        // click is after current paragraph
        if (visual_lines_consumed + visual_lines_of_paragraph <= click_position.row) {
            visual_lines_consumed += visual_lines_of_paragraph;
            current_paragraph++;
            continue;
        }

        int row_inside_paragraph = click_position.row - visual_lines_consumed;
        int column = std::min(
            row_inside_paragraph * context.text_area_size.width + click_position.column,
            static_cast<int>(state.getParagraph(current_paragraph).length())
        );

        return { current_paragraph, column };
    }

    // click after end of file
    int last = state.getNumberOfParagrahps() - 1;
    return {last, static_cast<int>(state.getParagraph(last).length())};
}

ParseResult EditorMode::parseMouseMovement(Position raw_click_position, ParsingContext context) {

    int aside_width = context.actual_size.width - context.text_area_size.width;

    if (!isClickPositionValid(raw_click_position, aside_width, context.text_area_size)) {
        return {ModeType::TOOL_MODE, {}};
    }

    raw_click_position.column -= aside_width;

    Position adjusted_position = convertToLogicPosition(raw_click_position, context); 

    return {std::nullopt,
        std::make_shared<FixedPositionMoveAction>(adjusted_position)};
}