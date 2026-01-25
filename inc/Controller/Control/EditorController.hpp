///
/// @file: EditorController.hpp
/// @description: description
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef EDITOR_CONTROLLER_HPP
#define EDITOR_CONTROLLER_HPP

#include <optional>

#include "../../Model/EditorState.hpp"
#include "../Mode/ModeManager.hpp"

class EditorController {
private:
    EditorState m_state;
    ModeManager m_mode_manager;
    
public:
    EditorController(std::optional<std::string> file_path);
    virtual ~EditorController() = default;

    int getCursorRow() const;
    int getCursorColumn() const;

    std::string getModeLabel() const;

    bool processInput(int input, ScreenSize size);
    
    int getLineCount() const;
    Position getFirstVisibleChar(ScreenSize size);
    std::string getPartialLine(Position start);
    const std::string& getLine(size_t row) const;

    int screenLineOfCursor(ScreenSize size);
};

#endif //EDITOR_CONTROLLER_HPP
