#include "../../../inc/Controller/Rendering/LineNumberRenderer.hpp"
#include "../../../inc/Shared/Utils/StringHelpers.hpp"

using std::vector, std::string;

LineNumberRenderer::LineNumberRenderer(const EditorState& state, const Settings& settings):
    c_state{state},
    c_settings{settings}
    {}

vector<VisualSegment> LineNumberRenderer::calculateLineNumbers(ScreenSize text_area_size) {
    if (!c_settings.isEnabled("do_numbering") == true) {
        return {};
    }
    
    vector<VisualSegment> numbers;

    int numbering_width = calculateLineNumberWidth();
    string empty_numbering(numbering_width - 1, ' ');
    empty_numbering += c_line_number_seperator;

    Position first_visible = c_state.getFirstVisibleChar(text_area_size);
    int current_paragraph = first_visible.row;
    bool is_first_paragraph = true;

    for (int visual_row = 0; visual_row < text_area_size.height;) {
        
        // end of file
        if (static_cast<size_t>(current_paragraph) >= c_state.getNumberOfParagrahps()) {
            numbers.push_back({empty_numbering, TextRole::MEDIUM_HIGHLIGHT, TextStyle::makeNormal()});
            visual_row++;
            continue;
        }
        
        bool drew_number = false;

        // render number in first line of paragraph, only if the start of the paragraph is visibe
        if (!is_first_paragraph || first_visible.column < text_area_size.width) {
            numbers.push_back(formatParagraphNumber(current_paragraph, numbering_width));
            visual_row++;
            drew_number = true;
        }
        
        // draw empty placeholder for other visible lines of paragraph
        int offscreen_characters = (is_first_paragraph? first_visible.column : 0);
        int lines_needed = TextFile::visualLinesNeeded(
            c_state.getParagraph(current_paragraph).length() - offscreen_characters,
            text_area_size.width
        );
        
        for (int _ = drew_number? 1 : 0; _ < lines_needed; _++) {
            visual_row++;
            if (visual_row <= text_area_size.height) {
                numbers.push_back({empty_numbering, TextRole::MEDIUM_HIGHLIGHT, TextStyle::makeNormal()});
            }   
        }
        
        current_paragraph++;
        is_first_paragraph = false;
    } 

    return numbers;
}

int LineNumberRenderer::calculateLineNumberWidth() {
    if (!c_settings.isEnabled("do_numbering") == true) {
        return 0;
    }

    const int c_min_number_area_width = 3;
    int longest_number_length = std::to_string(c_state.getNumberOfParagrahps() + 1).length();

    return std::max(c_min_number_area_width, longest_number_length + 1) + 1;
}

VisualSegment LineNumberRenderer::formatParagraphNumber(int current_paragraph, int line_number_width) {
    if (current_paragraph == c_state.getCursor().getRow()) {
        return formatCursorParagraphNumber(current_paragraph, line_number_width);
    }

    return formatNormalParagraphNumber(current_paragraph, line_number_width);
}

VisualSegment LineNumberRenderer::formatCursorParagraphNumber(int current_paragraph, int line_number_width) {
    std::string aligned_number = StringHelpers::leftAlign(
        std::to_string(current_paragraph + 1),
        line_number_width - 1
    );

    return {aligned_number + c_line_number_seperator, TextRole::MEDIUM_HIGHLIGHT, TextStyle::makeNormal()};
}

VisualSegment LineNumberRenderer::formatNormalParagraphNumber(int current_paragraph, int line_number_width) {
    int display_number;
    if (c_settings.isEnabled("do_relative_numbers") == true) {
        display_number = std::abs(current_paragraph - c_state.getCursor().getRow());
    }
    else {
        display_number = current_paragraph + 1;
    }

    return {
        StringHelpers::rightAlign(
            std::to_string(display_number) + c_line_number_seperator,
            line_number_width
        ),
        TextRole::MEDIUM_HIGHLIGHT, TextStyle::makeNormal()
    };
}
