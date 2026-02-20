#include "../../../inc/Controller/Modes/TypingMode.hpp"
#include "../../../inc/Controller/Actions/Editing/InsertAction.hpp"
#include "../../../inc/Controller/Actions/System/CompoundAction.hpp"
#include "../../../inc/Controller/Actions/Editing/DeleteAction.hpp"
#include "../../../inc/Controller/Actions/Structure/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Actions/Movement/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Actions/Movement/FixedPositionMoveAction.hpp"
#include "../../../inc/Controller/Actions/Editing/IndentAction.hpp"
#include "../../../inc/Controller/Actions/Editing/UnindentAction.hpp"

#include "../../../inc/Shared/Types/SpecialKey.hpp"

using std::make_shared;

ParseResult TypingMode::parseSpecialKey(SpecialKey key,
    ParsingContext context) {
    
    EditorState& state = context.state;
    Position cursor = state.getCursor().getPosition();

    switch (key) {
    case SpecialKey::ESCAPE: {
        return {ModeType::TOOL_MODE, {}};
    }

    case SpecialKey::BACKSPACE: {
        Position delete_position = cursor;
        if (delete_position.column > 0) { // move left if possible
            delete_position.column--;
        }
        else if (delete_position.row > 0) { // otherwise move to end of prev if possible
            delete_position.row--;
            delete_position.column = state.getParagraph(delete_position.row).length();
        } 
        else {
            return {std::nullopt, {}};
        }

        return {ModeType::TYPING_MODE, {std::make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            std::make_shared<DeleteAction>(delete_position, delete_position, cursor),
            // std::make_shared<CharwiseMoveAction>(context.text_area_size, Direction::LEFT)
        })}};
    }

    case SpecialKey::ENTER: {
        return {ModeType::TYPING_MODE, {std::make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            std::make_shared<ParagraphSplittingAction>(context.state.getCursor().getPosition()),
            std::make_shared<CharwiseMoveAction>(context.text_area_size, Direction::RIGHT)
        })}};

    }

    case SpecialKey::ARROW_LEFT: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::LEFT)
        }};
    }

    case SpecialKey::ARROW_DOWN: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::DOWN)
        }};
    }

    case SpecialKey::ARROW_UP: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::UP)
        }};
    }

    case SpecialKey::ARROW_RIGHT: {
        return {ModeType::TYPING_MODE, {
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::RIGHT)
        }};
    }

    case SpecialKey::TAB: {
        return {ModeType::TYPING_MODE, {
            make_shared<IndentAction>(cursor.row, context.settings.getTabWidth())
        }};
    }

    case SpecialKey::SHIFT_TAB: {
        return {ModeType::TYPING_MODE, {
            make_shared<UnindentAction>(context.state.getCursor().getRow(), context.settings.getTabWidth())
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
        return parseSpecialKey(*input.special_key, context);
    }

    if (input.standard_input.has_value()) {
        std::vector<std::string> content = {std::string(1, *input.standard_input)};
        return {ModeType::TYPING_MODE, 
            std::make_shared<InsertAction>(content, context.state.getCursor().getPosition()),
        };
    } 
    
    return {ModeType::TYPING_MODE, std::nullopt};
}