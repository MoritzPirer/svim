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

#include "../../View/UiHandler.hpp"
#include "../../Model/EditorState.hpp"
#include "../Mode/ModeManager.hpp"

class EditorController {
private:
    EditorState m_state;
    ModeManager m_mode_manager;
    UiHandler m_ui_handler;
      
    /// @brief calculates the position of the first character that should be visible on screen.
    ///     the calculation ensures that the cursor is always on the upper half of the screen
    /// @param size the current size of the text area 
    /// @return the position of the first visible character 
    Position getFirstVisibleChar(ScreenSize size);  

    /// @brief splits the given paragraph into a vector of chunks that are at
    ///     most as long as the text area is wide (i.e that don't need to wrap)
    /// @param paragraph the paragraph to split 
    /// @param start_column where in the paragraph to start (before that column is ignored) 
    /// @return the split vector
    std::vector<std::string> splitIntoRows(const std::string& paragraph, int start_column) const;

    /// @brief calculates the lines that are visible based on the position of the cursor
    std::vector<std::string> calculateVisibleRows();
    
    /// @brief calculates the position on the screen the cursor should be drawn to
    ///     accounting for off-screen lines and line wrapping
    Position calculateScreenPositionOfCursor();
    
    /// @brief calculates what should be rendered to the screen
    RenderInfo calculateRenderInfo();

public:
    EditorController(std::optional<std::string> file_path = std::nullopt);
    virtual ~EditorController() = default;

    /// @brief the main entry point for the editor
    void mainLoop();
};

#endif //EDITOR_CONTROLLER_HPP
