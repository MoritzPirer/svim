#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"

void SaveAction::applyTo(EditorState& state) {
    FileHandler::saveFile(state.getFile());
}