#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"

void ParagraphSplittingAction::applyTo(EditorState& state) {
    state.splitAtCursor();
    state.getFile().setHasUnsavedChanges(true);
}