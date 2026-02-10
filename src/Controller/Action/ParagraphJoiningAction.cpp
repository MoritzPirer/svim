#include "../../../inc/Controller/Action/ParagraphJoiningAction.hpp"

void ParagraphJoiningAction::applyTo(EditorState& state) {
    if (static_cast<size_t>(state.getCursor().getRow()) < state.getNumberOfParagrahps() - 1) {
        state.joinLineToPrevious(state.getCursor().getRow() + 1);
    }
}
