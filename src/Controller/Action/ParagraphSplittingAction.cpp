#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

void ParagraphSplittingAction::apply(ExecutionContext& context) {
    context.state.splitAtCursor();
}
