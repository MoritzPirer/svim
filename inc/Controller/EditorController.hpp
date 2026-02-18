///
/// @file: EditorController.hpp
/// @description: Organizes Data flow between Model and View
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef EDITOR_CONTROLLER_HPP
#define EDITOR_CONTROLLER_HPP

#include <optional>

#include "../View/UiHandler.hpp"
#include "../Model/EditorState.hpp"
#include "Modes/ModeManager.hpp"
#include "../Shared/Types/Scope.hpp"
#include "Services/UndoRedoManager.hpp"

#include "Services/Settings/Settings.hpp"

class EditorController {
private:
    EditorState m_state;
    ModeManager m_mode_manager;
    UiHandler m_ui_handler;
    Settings m_settings;
    UndoRedoManager m_UndoRedoManager;

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
