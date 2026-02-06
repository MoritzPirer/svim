///
/// @file: Renderer.hpp
/// @description: handles everything related to rendering the things to be shown on the screen
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "RenderChunk.hpp"
#include "../../Model/EditorState.hpp"
#include "../Mode/ModeManager.hpp"
#include "../Settings/Settings.hpp"

class Renderer {
private:
    const EditorState& m_state;
    const Settings& m_settings;
    const ModeManager& m_mode_manager;

    const char m_line_number_seperator = '|';

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

    /// @brief organize the metadata chunks into rows
    /// @return a vector where each element is a set of Chunks to be rendered in that row
    std::vector<std::vector<RenderChunk>> reorganizeMetadataRows(
        std::vector<RenderChunk> ordered_chunks, ScreenSize actual_size);

    /// @brief formats the paragraph number with the proper padding and absolute or relative number
    RenderChunk formatParagraphNumber(int current_paragraph, int line_number_width);
    
public:
    Renderer(const EditorState& state, const Settings& settings, const ModeManager& mode_manager);
    Renderer(const Renderer&) = default;
    ~Renderer() = default;

    /// @brief calculate how much horizontal space the line numbers need
    int calculateLineNumberWidth();

    /// @brief calculates the line numbers to show on the left edge of the text area
    /// @return the line numbers to render (empty if line numbers are turned of)
    std::vector<RenderChunk> calculateLineNumbers(ScreenSize text_area_size);
    
    /// @brief calculates the rows that should be shown for the metadata panel
    /// @param actual_size the total size of the screen (not only the text area)
    /// @return a vector where each element is a set of RenderChunks
    /// that should be rendered in the same row of the screen 
    std::vector<std::vector<RenderChunk>> calculateMetadataRows(ScreenSize actual_size);
    
    /// @brief calculates where on the screen the cursor should be rendered
    /// @param text_area_size the size of the text area
    Position calculateScreenPositionOfCursor(ScreenSize text_area_size);

    /// @brief calculates what lines are visible based on the position of the cursor and the size
    ///     of the text area
    /// @param text_area_size the size of the text area (i.e. the screen size without the metadata
    ///     panel and the line numbers)
    /// @return a vector where each line is a set of RenderChunks that should
    ///     be rendered in a line of the screen
    std::vector<std::vector<RenderChunk>> calculateVisibleRows(ScreenSize text_area_size);

};

#endif //RENDERER_HPP
