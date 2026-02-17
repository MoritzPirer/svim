#include "../../../inc/Controller/Mode/TypingMode.hpp"
#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"
#include "../../../inc/Controller/Action/IndentAction.hpp"
#include "../../../inc/Controller/Action/UnindentAction.hpp"

#include "../../../inc/Shared/SpecialKey.hpp"

using std::make_shared;

ParseResult TypingMode::parseSpecialKey(SpecialKey key,
    ScreenSize text_area_size, const Settings& settings, const EditorState& state) {

    switch (key) {
    case SpecialKey::ESCAPE: {
        return {ModeType::TOOL_MODE, {}};
    }

    case SpecialKey::BACKSPACE: {
        Position delete_position = state.getCursor().getPosition();
        if (delete_position.column > 0) { // move left if possible
            delete_position.column--;
        }
        else if (delete_position.row > 0) { // otherwise move to end of prev if possible
            delete_position.row--;
            delete_position.column = std::max(static_cast<int>(state.getParagraph(delete_position.row).length()) - 1, 0);
        } 
        else {
            return {std::nullopt, {}};
        }

        return {ModeType::TYPING_MODE, {std::make_shared<DeleteAction>(delete_position, delete_position)}};
    }

    case SpecialKey::ENTER: {
        return {ModeType::TYPING_MODE, {std::make_shared<ParagraphSplittingAction>()}};
    }

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

    case SpecialKey::TAB: {
        return {ModeType::TYPING_MODE, {
            std::make_shared<IndentAction>(settings.isEnabled("do_skinny_tabs")? 2 : 4)
        }};
    }

    case SpecialKey::SHIFT_TAB: {
        return {ModeType::TYPING_MODE, {
            std::make_shared<UnindentAction>(settings.isEnabled("do_skinny_tabs")? 2 : 4)
        }};
    }

    default: {
        return {ModeType::TYPING_MODE, {}};
    }

    }
}

ParseResult TypingMode::parseInput(
    Input input, ParsingContext context) {

    if (input.mouse_position.has_value()) {
        return parseMouseMovement(*input.mouse_position, context.actual_size, context.text_area_size);
    }

    if (input.special_key.has_value()) {
        return parseSpecialKey(*input.special_key, context.text_area_size, context.settings, context.state);
    }

    if (input.standard_input.has_value()) {
        std::vector<std::string> content = {std::string(1, *input.standard_input)};
        return {ModeType::TYPING_MODE, 
            std::make_shared<InsertAction>(content, context.state.getCursor().getPosition()),
        };
    } 
    
    return {ModeType::TYPING_MODE, std::nullopt};
}