///
/// @file: LineNumberRenderer.hpp
/// @description: description
///
/// @date: 2026-03-16
/// @author: Moritz Pirer
///

#ifndef LINE_NUMBER_RENDERER_HPP
#define LINE_NUMBER_RENDERER_HPP

#include "../../Shared/DataFlow/VisualSegment.hpp"
#include "../../Model/EditorState.hpp"
#include "../Services/Settings/Settings.hpp"
#include "../../Shared/Types/ScreenSize.hpp"

class LineNumberRenderer {
private:
    const EditorState& c_state;
    const Settings& c_settings;
    const char c_line_number_seperator = '|';

    /// @brief formats the given paragraph number as the paragraph containing the cursor (i.e. left-aligned)
    /// @param current_paragraph the 0-indexed paragraph number
    /// @param line_number_width the width to pad to
    VisualSegment formatCursorParagraphNumber(int current_paragraph, int line_number_width);

    /// @brief formats the given paragraph number as a paragraph not containing the cursor (i.e. right-aligned)
    /// @param current_paragraph the 0-indexed paragraph number
    /// @param line_number_width the width to pad to
    VisualSegment formatNormalParagraphNumber(int current_paragraph, int line_number_width);
    
    /// @brief formats the paragraph number with the proper padding and absolute or relative numbe
    VisualSegment formatParagraphNumber(int current_paragraph, int line_number_width);

public:
    LineNumberRenderer(const EditorState& state, const Settings& settings);
    LineNumberRenderer(const LineNumberRenderer&) = default;
    ~LineNumberRenderer() = default;

    /// @brief calculate how much horizontal space the line numbers need
    int calculateLineNumberWidth();

    /// @brief calculates the line numbers to show on the left edge of the text area
    /// @return the line numbers to render (empty if line numbers are turned of)
    std::vector<VisualSegment> calculateLineNumbers(ScreenSize text_area_size);
};

#endif //LINE_NUMBER_RENDERER_HPP
