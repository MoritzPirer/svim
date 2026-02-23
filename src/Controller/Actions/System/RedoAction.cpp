#include "../../../../inc/Controller/Actions/System/RedoAction.hpp"
#include "../../../../inc/Controller/Services/UndoRedoManager.hpp"

#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

void RedoAction::apply(ExecutionContext& context) {
    context.undoRedoManager.redo(context);
}
