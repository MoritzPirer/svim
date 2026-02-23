#include "../../../../inc/Controller/Actions/System/UndoAction.hpp"
#include "../../../../inc/Controller/Services/UndoRedoManager.hpp"

#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

void UndoAction::apply(ExecutionContext& context) {
    context.undoRedoManager.undo(context.state);
}
