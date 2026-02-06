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
#include "../../Shared/Scope.hpp"

#include "../Settings/Settings.hpp"

class EditorController {
private:
    EditorState m_state;
    ModeManager m_mode_manager;
    UiHandler m_ui_handler;
    Settings m_settings;
      
    /// @brief calculates the lines that are visible based on the position of the cursor
    std::vector<std::vector<RenderChunk>> calculateVisibleRows(ScreenSize text_are_size);
    
    /// @brief calculates the position on the screen the cursor should be drawn to
    ///     accounting for off-screen lines and line wrapping
    Position calculateScreenPositionOfCursor(ScreenSize text_area_size);
   
    /// @brief calculates what should be rendered to the screen
    RenderInfo calculateRenderInfo(ScreenSize actual_size);

    ScreenSize calculateTextAreaSize(const RenderInfo& render_info, const ScreenSize& total_size);
    
public:
    EditorController(std::optional<std::string> file_path = std::nullopt);
    virtual ~EditorController() = default;

    /// @brief the main entry point for the editor
    void mainLoop();
};

#endif //EDITOR_CONTROLLER_HPP
