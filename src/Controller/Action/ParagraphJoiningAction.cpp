#include "../../../inc/Controller/Action/ParagraphJoiningAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

void ParagraphJoiningAction::apply(ExecutionContext& context) {
    int row = context.state.getCursor().getRow();

    if (static_cast<size_t>(row) < context.state.getNumberOfParagrahps() - 1) {
        context.state.joinLineToPrevious(row + 1);
    }
}
