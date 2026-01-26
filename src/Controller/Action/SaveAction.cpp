#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"

void SaveAction::applyTo(EditorState& state) {
    FileHandler file_handler;
    file_handler.saveFile(state.getFile());
}