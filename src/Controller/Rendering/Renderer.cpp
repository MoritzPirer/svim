#include <algorithm>

#include "../../../inc/Controller/Rendering/Renderer.hpp"
#include "../../../inc/Shared/Utils/StringHelpers.hpp"
#include "../../../inc/Shared/Utils/Version.hpp"

using std::vector, std::string;

Renderer::Renderer(const EditorState& state, const Settings& settings, const ModeManager& mode_manager):
    m_state{state},
    m_settings{settings},
    m_mode_manager{mode_manager}
    {}

VisualSegment Renderer::formatCurrentParagraphNumber(int current_paragraph, int line_number_width) {
    std::string aligned_number = StringHelpers::leftAlign(
        std::to_string(current_paragraph + 1),
        line_number_width - 1
    );

    return {aligned_number + c_line_number_seperator, TextRole::UI_ELEMENT};
}

VisualSegment Renderer::formatNonCurrentParagraphNumber(int current_paragraph, int line_number_width) {
    int display_number;
    if (m_settings.isEnabled("do_relative_numbers") == true) {
        display_number = std::abs(current_paragraph - m_state.getCursor().getRow());
    }
    else {
        display_number = current_paragraph + 1;
    }

    return {
        StringHelpers::rightAlign(
            std::to_string(display_number) + c_line_number_seperator,
            line_number_width
        ),
        TextRole::UI_ELEMENT
    };
}

VisualSegment Renderer::formatParagraphNumber(int current_paragraph, int line_number_width) {
    if (current_paragraph == m_state.getCursor().getRow()) {
        return formatCurrentParagraphNumber(current_paragraph, line_number_width);
    }

    return formatNonCurrentParagraphNumber(current_paragraph, line_number_width);
}

int Renderer::calculateLineNumberWidth() {
    if (!m_settings.isEnabled("do_numbering") == true) {
        return 0;
    }

    const int c_min_number_area_width = 3;
    int longest_number_length = std::to_string(m_state.getNumberOfParagrahps() + 1).length();

    return std::max(c_min_number_area_width, longest_number_length + 1) + 1;
}

vector<VisualSegment> Renderer::calculateLineNumbers(ScreenSize text_area_size) {
    if (!m_settings.isEnabled("do_numbering") == true) {
        return {};
    }
    
    vector<VisualSegment> numbers;

    int numbering_width = calculateLineNumberWidth();
    string empty_numbering(numbering_width - 1, ' ');
    empty_numbering += c_line_number_seperator;

    Position first_visible = m_state.getFirstVisibleChar(text_area_size);
    int current_paragraph = first_visible.row;
    bool is_first_paragraph = true;

    for (int visual_row = 0; visual_row < text_area_size.height;) {
        
        // end of file
        if (static_cast<size_t>(current_paragraph) >= m_state.getNumberOfParagrahps()) {
            numbers.push_back({empty_numbering, TextRole::UI_ELEMENT});
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
            m_state.getParagraph(current_paragraph).length() - offscreen_characters,
            text_area_size.width
        );
        
        for (int _ = drew_number? 1 : 0; _ < lines_needed; _++) {
            visual_row++;
            if (visual_row <= text_area_size.height) {
                numbers.push_back({empty_numbering, TextRole::UI_ELEMENT});
            }   
        }
        
        current_paragraph++;
        is_first_paragraph = false;
    } 

    return numbers;
}

vector<VisualSegment> Renderer::getSeperatorChunks(ScreenSize actual_size) {
    return { VisualSegment{
        string(actual_size.width - 1, (m_settings.isEnabled("show_seperator") ? '-' : ' ')),
        TextRole::UI_ELEMENT
    }};
}

vector<VisualSegment> Renderer::getCharacterCountChunks() {
    if (!m_settings.isEnabled("show_character_count")) {
        return {};
    }

    int character_count = m_state.getNumberOfCharacters();
    std::string count_string = StringHelpers::addSeperators(character_count, 3);

    return {
        {"Character(s): ", TextRole::TEXT_NORMAL},
        {count_string, TextRole::TEXT_HIGHLIGHT}
    };
}

vector<VisualSegment> Renderer::getWordCountChunks() {
    if (!m_settings.isEnabled("show_word_count")) {
        return {};
    }

    int word_count = m_state.getNumberOfWords();
    std::string count_string = StringHelpers::addSeperators(word_count, 3);

    return {
        {"Word(s): ", TextRole::TEXT_NORMAL},
        {count_string, TextRole::TEXT_HIGHLIGHT}
    };
}

vector<VisualSegment> Renderer::getParagraphCountChunks() {
    if (!m_settings.isEnabled("show_paragraph_count")) {
        return {};
    }

    int paragraph_count = m_state.getNumberOfParagrahps();
    std::string count_string = StringHelpers::addSeperators(paragraph_count, 3);

    return {
        {"Paragraph(s): ", TextRole::TEXT_NORMAL},
        {count_string, TextRole::TEXT_HIGHLIGHT}
    };
}

vector<VisualSegment> Renderer::getCursorPositionChunks() {
    if (!m_settings.isEnabled("show_cursor_position")) {
        return {};
    }

    Position position = m_state.getCursor().getPosition();
    position.row++; // display as 1-indexed

    return {
        {"Cursor position: ", TextRole::TEXT_NORMAL},
        {position.format(), TextRole::TEXT_HIGHLIGHT}
    };
}

vector<VisualSegment> Renderer::getFileNameChunks() {
    if (!m_settings.isEnabled("show_file_name")) {
        return {};
    }

    return {{"Currently editing " + m_state.getFileName(), TextRole::TEXT_NORMAL}};
}

vector<VisualSegment> Renderer::getSaveIconChunks() {
    bool show_save_icon = m_settings.isEnabled("show_save_icon");
    bool show_save_text = m_settings.isEnabled("show_save_text");

    if (!show_save_icon && !show_save_text) {
        return {};
    }
    
    string icon = "";
    string text = "";
    TextRole role;    

    switch (m_state.getSaveState()) {
    case SaveState::NEW_FILE: {
        icon = "[!]";
        text = "New File";
        role = TextRole::FILE_NEW;
        break;
    }

    case SaveState::UNSAVED_CHANGES: {
        icon = "[*]";
        text = "Unsaved Changes";
        role = TextRole::FILE_CHANGED;
        break;
    }

    case SaveState::SAVED: {
        icon = "[=]";
        text = "Changes Saved";
        role = TextRole::FILE_SAVED;
    }

    }

    string message = icon + (show_save_icon && show_save_text? " " : "") + text;

    return {{message, role}};
}

vector<VisualSegment> Renderer::getVersionChunks() {
    if (!m_settings.isEnabled("show_version")) {
        return {};
    }

    return {{VERSION, TextRole::TEXT_NORMAL}};
}


vector<VisualSegment> Renderer::getEditorModeChunks() {
    if (!m_settings.isEnabled("show_editor_mode")) {
        return {};
    }

    return {{m_mode_manager.getModeLabel(), TextRole::TEXT_HIGHLIGHT}};
}

vector<vector<VisualSegment>> Renderer::reorganizeMetadataRows(
    vector<VisualSegment> ordered_chunks, ScreenSize actual_size) {
    
    int row_length = 0;
    vector<vector<VisualSegment>> rows;
    vector<VisualSegment> current_row;

    int max_used_width = actual_size.width - 1;

    for (const VisualSegment& chunk : ordered_chunks) {
        if (row_length + static_cast<int>(chunk.content.size()) > max_used_width) {
            rows.push_back(current_row);
            current_row.clear();
            row_length = 0;
        }

        current_row.push_back(chunk);
        row_length += chunk.content.size();
    }

    if (!current_row.empty()) {
        rows.push_back(current_row);
    }

    return rows; 
}

vector<vector<VisualSegment>> Renderer::calculateMetadataRows(ScreenSize actual_size) {
    vector<VisualSegment> ordered_chunks; 

    auto addContent = [&](const vector<VisualSegment>& chunks, bool supress_divider = false) {
        ordered_chunks.insert(ordered_chunks.end(), chunks.begin(), chunks.end());
        if (chunks.size() != 0 && !supress_divider) {
            ordered_chunks.push_back({" | ", TextRole::TEXT_NORMAL});
        }
    };
    
    addContent(getSeperatorChunks(actual_size), true);
    addContent(getEditorModeChunks());
    addContent(getFileNameChunks());
    addContent(getSaveIconChunks());
    addContent(getCursorPositionChunks());
    addContent(getCharacterCountChunks());
    addContent(getWordCountChunks());
    addContent(getParagraphCountChunks());
    addContent(getVersionChunks());

    return reorganizeMetadataRows(ordered_chunks, actual_size);
}

Position Renderer::calculateScreenPositionOfCursor(ScreenSize text_area_size) {
    int screen_row = std::min(
        text_area_size.height / 2, // keep cursor in top half of screen
        m_state.calculateVisualLineOfCursor(text_area_size.width)
    ); 

    int screen_column = m_state.getCursor().getColumn() % text_area_size.width;

    return {screen_row, screen_column};
}

vector<vector<VisualSegment>> Renderer::renderTextNormal(const vector<string> split_paragraph, int visual_rows_available) {
    vector<vector<VisualSegment>> segments;
    for (const string& line : split_paragraph) {
        if (visual_rows_available > 0) {
            segments.push_back({VisualSegment{
                line,
                TextRole::TEXT_NORMAL
            }});
        }
        
        visual_rows_available--;
    }

    return segments;
}

bool Renderer::isFollowedByUnderline(int paragraph_index) {
    if (m_state.isLastParagraph(paragraph_index)) {
        return false;
    }

    const std::string& next = m_state.getParagraph(paragraph_index + 1);

    return (StringHelpers::consistsOnlyOfIgnoringWhitespace(next, c_underline_indicator)
        && std::ranges::count(next, c_underline_indicator) >= c_min_underline_count);
}

bool Renderer::isHeading(int paragraph_index) {
    return isFollowedByUnderline(paragraph_index)
        || StringHelpers::startsWith(m_state.getParagraph(paragraph_index), c_heading_indicator);
}

bool Renderer::isQuote(int paragraph_index) {
    return StringHelpers::startsWithIgnoringWhitespace(m_state.getParagraph(paragraph_index), c_quote_indicator);
}

vector<vector<VisualSegment>> Renderer::renderFullLineHighlight(vector<string> split_paragraph, int max_width,
    int current_paragraph, int visual_rows_available) {

    TextRole text_role = TextRole::TEXT_NORMAL;

    if (isHeading(current_paragraph)) {
        text_role = TextRole::TEXT_HEADING;
    }
    else if (isQuote(current_paragraph)) {
        text_role = TextRole::TEXT_QUOTE;
        for (string& line : split_paragraph) {
            line = StringHelpers::leftAlign(line, max_width);
        }
    }

    vector<vector<VisualSegment>> segments;
    for (const string& line : split_paragraph) {
        if (visual_rows_available == 0) {
            break;
        }

        if (visual_rows_available > 0) {
            segments.push_back({VisualSegment{
                line,
                text_role
            }});
        }
        
        visual_rows_available--;
    }

    return segments;
}

vector<vector<VisualSegment>> Renderer::calculateVisibleRows(ScreenSize text_area_size) {
    Position first_visible = m_state.getFirstVisibleChar(text_area_size);

    int current_paragraph = first_visible.row;
    vector<vector<VisualSegment>> visible_rows;    
    visible_rows.reserve(text_area_size.height);

    bool do_syntax_highlighting = (m_state.getFileName().ends_with(".md"));

    bool is_first_paragraph = true;
    for (int visual_row = 0; visual_row < text_area_size.height;) {

        if (static_cast<size_t>(current_paragraph) >= m_state.getNumberOfParagrahps()) { 
            visible_rows.push_back({VisualSegment{
                "~",
                TextRole::TEXT_HIGHLIGHT
            }}); // FUTURE: load placeholder line from settings

            visual_row++;
            continue;
        }

        vector<string> split = StringHelpers::splitIntoRows(
            m_state.getParagraph(current_paragraph),
            (is_first_paragraph? first_visible.column : 0),
            text_area_size.width 
        );
        
        is_first_paragraph = false;

        vector<vector<VisualSegment>> temp;
        if (do_syntax_highlighting) {
            temp = renderFullLineHighlight(split,
                text_area_size.width,
                current_paragraph,
                text_area_size.height - visual_row
            );
        }
        else {
            temp = renderTextNormal(split,
                text_area_size.height - visual_row
            );
        }

        visible_rows.insert(visible_rows.end(), temp.begin(), temp.end());
        visual_row += split.size();
        current_paragraph++;
    }
    
    return visible_rows;
}

vector<VisualSegment> Renderer::calculateTemporaryRows(ScreenSize actual_size) {
    vector<VisualSegment> chunks;

    if (!m_state.getTemporaryMessages().empty()) {
        vector<VisualSegment> seperator_chunks = getSeperatorChunks(actual_size);
        chunks.insert(chunks.end(), seperator_chunks.begin(), seperator_chunks.end());
    }

    for (const string& message : m_state.getTemporaryMessages()) {
        for (const string& message_row : StringHelpers::splitIntoRows(message, 0, actual_size.width - 1)) {
            chunks.push_back({
                StringHelpers::leftAlign(message_row, actual_size.width),
                TextRole::TEXT_HIGHLIGHT
            });
        }
    }

    return chunks;
}