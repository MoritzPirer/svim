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
      
    /// @brief one true source for editor version
    std::string getVersion() const { return "version 0.2.1"; }

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
    std::vector<std::string> splitIntoRows(const std::string& paragraph,
        int start_column, int max_length) const;

    /// @brief calculates the lines that are visible based on the position of the cursor
    std::vector<std::vector<RenderChunk>> calculateVisibleRows(ScreenSize text_are_size);
    
    /// @brief calculates the position on the screen the cursor should be drawn to
    ///     accounting for off-screen lines and line wrapping
    Position calculateScreenPositionOfCursor(ScreenSize text_area_size);
   
    std::vector<RenderChunk> getSeperatorChunks(ScreenSize actual_size);
    std::vector<RenderChunk> getTemporaryMessageChunks(ScreenSize actual_size);
    std::vector<RenderChunk> getCharacterCountChunks();
    std::vector<RenderChunk> getWordCountChunks();
    std::vector<RenderChunk> getParagraphCountChunks();
    std::vector<RenderChunk> getCursorPositionChunks();
    std::vector<RenderChunk> getEditorModeChunks();
    std::vector<RenderChunk> getFileNameChunks();
    std::vector<RenderChunk> getSaveIconChunks();
    std::vector<RenderChunk> getVersionChunks();

    std::vector<std::vector<RenderChunk>> reorganizeMetadataRows(
        std::vector<RenderChunk> ordered_chunks, ScreenSize actual_size);

    std::vector<std::vector<RenderChunk>> calculateMetadataRows(ScreenSize actual_size);
    /// @brief calculates what should be rendered to the screen
    RenderInfo calculateRenderInfo(ScreenSize actual_size);

public:
    EditorController(std::optional<std::string> file_path = std::nullopt);
    virtual ~EditorController() = default;

    /// @brief the main entry point for the editor
    void mainLoop();
};

#endif //EDITOR_CONTROLLER_HPP
