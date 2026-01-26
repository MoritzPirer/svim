#include <ncurses.h>
#include "../../../inc/Controller/Control/EditorController.hpp"
#include "../../../inc/Controller/Control/FileHandler.hpp"

EditorController::EditorController(std::optional<std::string> file_path):
    m_mode_manager{ModeType::TOOL_MODE} {

    FileHandler file_handler;
    TextFile file;
    if (!file_path.has_value() || file_path->empty()) {
        std::filesystem::path name = file_handler.getDefaultName(); 
        file = file_handler.createFile(name);
    }
    else {
        file = file_handler.openFile(file_path.value());      
    }
    m_state = EditorState(file);
}

int EditorController::getCursorRow() const {
    return m_state.getCursor().getPosition().row;
}

int EditorController::getCursorColumn() const {
    return m_state.getCursor().getPosition().column;
}

std::string EditorController::getModeLabel() const {
    return m_mode_manager.getModeLabel();
}
bool EditorController::processInput(int input, ScreenSize size) {
    if (input == 'q' ) { //Temporary until quit command exists properly
        return true;
    }
    
    std::shared_ptr<Action> action = m_mode_manager.convertToAction(input, size);
    action->applyTo(m_state);

    return false;
}

Position EditorController::getFirstVisibleChar(ScreenSize size) {
    return m_state.getFirstVisibleChar(size);
}

int EditorController::getLineCount() const {
    return m_state.getLineCount();
}

std::string EditorController::getPartialLine(Position start) {
    return m_state.getPartialLine(start);
}

const std::string& EditorController::getLine(size_t row) const {
    return m_state.getLine(row);
}

int EditorController::screenLineOfCursor(ScreenSize size) {
    int visual_line = m_state.calculateVisualLineOfCursor(size.width);


    return std::min(size.height / 2 - 1, visual_line - 1);
}