#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Controller/Control/FileException.hpp"

#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

SaveAction::SaveAction(SaveConfirmation confirm_save):
    m_confirm_save{confirm_save} {}

void SaveAction::apply(ExecutionContext& context) {
    try {
        FileHandler::saveFile(context.state.getFile());

        if (m_confirm_save == SaveConfirmation::YES) {
            context.state.addTemporaryMessage("Saved all changes!");
        }
    }
    catch (FileException& e) {
        context.state.addTemporaryMessage("Saving failed!");
    }

}