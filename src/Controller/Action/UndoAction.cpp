#include "../../../inc/Controller/Action/UndoAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"
#include "../../../inc/Controller/Control/History.hpp"

void UndoAction::apply(ExecutionContext& context) {
    context.history.undo(context.state);
}
