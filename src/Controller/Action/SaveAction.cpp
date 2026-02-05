#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"

SaveAction::SaveAction(bool confirm_save):
    m_confirm_save{confirm_save} {}

void SaveAction::applyTo(EditorState& state) {
    FileHandler::saveFile(state.getFile());

    if (m_confirm_save) {
        state.addTemporaryMessage("Saved all changes!");
    }
}