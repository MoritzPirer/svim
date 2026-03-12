///
/// @file: Renderer.hpp
/// @description: handles everything related to rendering the things to be shown on the screen
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../../Shared/DataFlow/VisualSegment.hpp"
#include "../../Model/EditorState.hpp"
#include "../Modes/ModeManager.hpp"
#include "../Services/Settings/Settings.hpp"

class Renderer {
private:
    const EditorState& m_state;
    const Settings& m_settings;
    const ModeManager& m_mode_manager;

    const char c_line_number_seperator = '|';
    const char c_underline_indicator = '-';
    const char c_min_underline_count = 2;
    const char c_heading_indicator = '#';
    const char c_quote_indicator = '>';

    std::vector<VisualSegment> getSeperatorChunks(ScreenSize actual_size);
    std::vector<VisualSegment> getCharacterCountChunks();
    std::vector<VisualSegment> getWordCountChunks();
    std::vector<VisualSegment> getParagraphCountChunks();
    std::vector<VisualSegment> getCursorPositionChunks();
    std::vector<VisualSegment> getEditorModeChunks();
    std::vector<VisualSegment> getFileNameChunks();
    std::vector<VisualSegment> getSaveIconChunks();
    std::vector<VisualSegment> getVersionChunks();

    /// @brief organize the metadata chunks into rows
    /// @return a vector where each element is a set of Chunks to be rendered in that row
    std::vector<std::vector<VisualSegment>> reorganizeMetadataRows(
        std::vector<VisualSegment> ordered_chunks, ScreenSize actual_size);

    VisualSegment formatCurrentParagraphNumber(int current_paragraph, int line_number_width);

    VisualSegment formatNonCurrentParagraphNumber(int current_paragraph, int line_number_width);
    
    /// @brief formats the paragraph number with the proper padding and absolute or relative number
    VisualSegment formatParagraphNumber(int current_paragraph, int line_number_width);

    bool isFollowedByUnderline(int paragraph_index);
    bool isHeading(int paragraph_index);
    bool isQuote(int paragraph_index);

    TextRole getTextRole(int current_paragraph);

    std::vector<std::vector<VisualSegment>> renderHighlights(std::vector<std::string> split_paragraph, int max_width,
    int current_paragraph, int visual_rows_available, int first_visible);

public:
    Renderer(const EditorState& state, const Settings& settings, const ModeManager& mode_manager);
    Renderer(const Renderer&) = default;
    ~Renderer() = default;

    /// @brief calculate how much horizontal space the line numbers need
    int calculateLineNumberWidth();

    /// @brief calculates the line numbers to show on the left edge of the text area
    /// @return the line numbers to render (empty if line numbers are turned of)
    std::vector<VisualSegment> calculateLineNumbers(ScreenSize text_area_size);
    
    /// @brief calculates the rows that should be shown for the metadata panel
    /// @param actual_size the total size of the screen (not only the text area)
    /// @return a vector where each element is a set of VisualSegments
    /// that should be rendered in the same row of the screen 
    std::vector<std::vector<VisualSegment>> calculateMetadataRows(ScreenSize actual_size);
    
    /// @brief calculates where on the screen the cursor should be rendered
    /// @param text_area_size the size of the text area
    Position calculateScreenPositionOfCursor(ScreenSize text_area_size);

    /// @brief calculates what lines are visible based on the position of the cursor and the size
    ///     of the text area
    /// @param text_area_size the size of the text area (i.e. the screen size without the metadata
    ///     panel and the line numbers)
    /// @return a vector where each line is a set of VisualSegments that should
    ///     be rendered in a line of the screen
    std::vector<std::vector<VisualSegment>> calculateVisibleRows(ScreenSize text_area_size);

    std::vector<VisualSegment> calculateTemporaryRows(ScreenSize actual_size);

};

#endif //RENDERER_HPP
