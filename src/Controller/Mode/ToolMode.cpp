#include <ncurses.h>
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"
#include "../../../inc/Controller/Action/ChunkwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/DirectionalMoveAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"

#include "../../../inc/Controller/Settings/Settings.hpp"
#include "../../../inc/Shared/SpecialInputs.hpp"

using std::make_shared;

std::pair<ModeType, std::vector<std::shared_ptr<Action>>> ToolMode::parseInput(
    int input, ScreenSize size, Settings settings) {
    switch (input) {
        // move actions
        case ARROW_LEFT:
        case 'h':
            return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(size, Direction::BACKWARD)}};
        case ARROW_DOWN:
        case 'j':
            return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(size, Direction::DOWN)}};
        case ARROW_UP:
        case 'k':
            return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(size, Direction::UP)}};
        case ARROW_RIGHT:
        case 'l':
            return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(size, Direction::FORWARD)}};
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
                make_shared<DirectionalMoveAction>(Scope::WORD, Destination::START, size, Direction::FORWARD)
            }};
        case 'W':
            return {ModeType::TOOL_MODE, {
                make_shared<DirectionalMoveAction>(Scope::EXPRESSION, Destination::START, size, Direction::FORWARD)
            }};
        case 'b':
            return {ModeType::TOOL_MODE, {
                make_shared<DirectionalMoveAction>(Scope::WORD, Destination::START, size, Direction::BACKWARD)
            }};
        case 'B':
            return {ModeType::TOOL_MODE, {
                make_shared<DirectionalMoveAction>(Scope::EXPRESSION, Destination::START, size, Direction::BACKWARD)
            }};
        case 'e':
            return {ModeType::TOOL_MODE, {
                make_shared<DirectionalMoveAction>(Scope::WORD, Destination::END, size, Direction::FORWARD)
            }};
        case 'E':
            return {ModeType::TOOL_MODE, {
                make_shared<DirectionalMoveAction>(Scope::EXPRESSION, Destination::END, size, Direction::FORWARD)
            }};

        // mode switching actions
        case 'i':
            return {ModeType::TYPING_MODE, {}};
        case 'a':
            return {ModeType::TYPING_MODE, {make_shared<CharwiseMoveAction>(size, Direction::FORWARD)}};
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
                make_shared<CharwiseMoveAction>(size, Direction::UP)
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

}