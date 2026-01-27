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

void EditorController::processInput(int input, ScreenSize size) {
    std::vector<std::shared_ptr<Action>> actions = m_mode_manager.convertToAction(input, size);
    for (std::shared_ptr action : actions) {
        action->applyTo(m_state);
    }
}

Position EditorController::getFirstVisibleChar(ScreenSize size) {
    return m_state.getFirstVisibleChar(size);
}

bool EditorController::isQuit() const {
    return m_state.getIsQuit();
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