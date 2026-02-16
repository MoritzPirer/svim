#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

SaveAction::SaveAction(SaveConfirmation confirm_save):
    m_confirm_save{confirm_save} {}

void SaveAction::apply(ExecutionContext& context) {
    //TODO check if save succeeded (via exception? and give message if it failed)
    FileHandler::saveFile(context.state.getFile());

    if (m_confirm_save == SaveConfirmation::YES) {
        context.state.addTemporaryMessage("Saved all changes!");
    }
}