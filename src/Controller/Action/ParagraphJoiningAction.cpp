#include "../../../inc/Controller/Action/ParagraphJoiningAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

void ParagraphJoiningAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;
    if (static_cast<size_t>(state.getCursor().getRow()) < state.getNumberOfParagrahps() - 1) {
        state.joinLineToPrevious(state.getCursor().getRow() + 1);
    }
}
