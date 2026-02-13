#include "../../../inc/Controller/Control/Renderer.hpp"
#include "../../../inc/Shared/StringHelpers.hpp"
#include "../../../inc/Controller/Control/Version.hpp"

using std::vector, std::string;

Renderer::Renderer(const EditorState& state, const Settings& settings, const ModeManager& mode_manager):
    m_state{state},
    m_settings{settings},
    m_mode_manager{mode_manager}
    {}

RenderChunk Renderer::formatParagraphNumber(int current_paragraph, int line_number_width) {
     if (current_paragraph == m_state.getCursor().getRow()) {
        return {
            StringHelpers::leftAlign(std::to_string(current_paragraph + 1), line_number_width - 1)
                + m_line_number_seperator,
            TextRole::UI_ELEMENT
        };
    }

    int display_number;
    if (m_settings.isEnabled("do_relative_numbers") == true) {
        display_number = std::abs(current_paragraph - m_state.getCursor().getRow());
    }
    else {
        display_number = current_paragraph + 1;
    }

    return {
        StringHelpers::rightAlign(
            std::to_string(display_number) + m_line_number_seperator,
            line_number_width
        ),
        TextRole::UI_ELEMENT
    };
}

int Renderer::calculateLineNumberWidth() {
    if (!m_settings.isEnabled("do_numbering") == true) {
        return 0;
    }

    int longest_number_length = std::to_string(m_state.getNumberOfParagrahps() + 1).length();
    int min_number_area_width = 3;

    return std::max(min_number_area_width, longest_number_length + 1) + 1;
}

vector<RenderChunk> Renderer::calculateLineNumbers(ScreenSize text_area_size) {
    if (!m_settings.isEnabled("do_numbering") == true) {
        return {};
    }
    
    vector<RenderChunk> numbers;

    int numbering_width = calculateLineNumberWidth();
    string empty_numbering(numbering_width - 1, ' ');
    empty_numbering += '|';

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

vector<RenderChunk> Renderer::getSeperatorChunks(ScreenSize actual_size) {

    return { RenderChunk{
        string(actual_size.width - 1, (m_settings.isEnabled("show_seperator") ? '-' : ' ')),
        TextRole::UI_ELEMENT
    }};
}

vector<RenderChunk> Renderer::getCharacterCountChunks() {
    if (!m_settings.isEnabled("show_character_count")) {
        return {};
    }

    int character_count = m_state.getNumberOfCharacters();
    return {
        {"Character(s): ", TextRole::TEXT_NORMAL},
        {std::to_string(character_count), TextRole::TEXT_HIGHLIGHT}
    };
}

vector<RenderChunk> Renderer::getWordCountChunks() {
    if (!m_settings.isEnabled("show_word_count")) {
        return {};
    }

    int word_count = m_state.getNumberOfWords();
    return {
        {"Word(s): ", TextRole::TEXT_NORMAL},
        {std::to_string(word_count), TextRole::TEXT_HIGHLIGHT}
    };
}

vector<RenderChunk> Renderer::getParagraphCountChunks() {
    if (!m_settings.isEnabled("show_paragraph_count")) {
        return {};
    }

    int paragraph_count = m_state.getNumberOfParagrahps();
    return {
        {"Paragraph(s): ", TextRole::TEXT_NORMAL},
        {std::to_string(paragraph_count), TextRole::TEXT_HIGHLIGHT}
    };
}

vector<RenderChunk> Renderer::getCursorPositionChunks() {
    if (!m_settings.isEnabled("show_cursor_position")) {
        return {};
    }
    Position position = m_state.getCursor().getPosition();
    position.row++;
    return {
        {"Cursor position: ", TextRole::TEXT_NORMAL},
        {position.format(), TextRole::TEXT_HIGHLIGHT}
    };
}

vector<RenderChunk> Renderer::getFileNameChunks() {
    if (!m_settings.isEnabled("show_file_name")) {
        return {};
    }

    return {{"Currently editing " + m_state.getFileName(), TextRole::TEXT_NORMAL}};
}

vector<RenderChunk> Renderer::getSaveIconChunks() {
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

vector<RenderChunk> Renderer::getVersionChunks() {
    if (!m_settings.isEnabled("show_version")) {
        return {};
    }

    return {{VERSION, TextRole::TEXT_NORMAL}};
}


vector<RenderChunk> Renderer::getEditorModeChunks() {
    if (!m_settings.isEnabled("show_editor_mode")) {
        return {};
    }

    return {{m_mode_manager.getModeLabel(), TextRole::TEXT_HIGHLIGHT}};
}

vector<vector<RenderChunk>> Renderer::reorganizeMetadataRows(
    vector<RenderChunk> ordered_chunks, ScreenSize actual_size) {
    
    int row_length = 0;
    int chunks_in_row = 0;
    vector<vector<RenderChunk>> rows;
    vector<RenderChunk> current_row;

    int max_used_width = actual_size.width - 1;
    int max_chunks_in_row = 900;

    for (const RenderChunk& chunk : ordered_chunks) {
        if (row_length + static_cast<int>(chunk.content.size()) > max_used_width
            || chunks_in_row >= max_chunks_in_row) {
            rows.push_back(current_row);

            current_row.clear();
            row_length = 0;
            chunks_in_row = 0;
        }

        current_row.push_back(chunk);
        row_length += chunk.content.size();
        chunks_in_row++;
    }

    if (!current_row.empty()) {
        rows.push_back(current_row);
    }

    return rows; 
}

vector<vector<RenderChunk>> Renderer::calculateMetadataRows(ScreenSize actual_size) {
    vector<RenderChunk> ordered_chunks; 

    auto addContent = [&](const vector<RenderChunk>& chunks, bool supress_divider = false) {
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
        text_area_size.height / 2,
        m_state.calculateVisualLineOfCursor(text_area_size.width)
    ); 

    int screen_column = m_state.getCursor().getColumn() % text_area_size.width;

    return {screen_row, screen_column};
}

vector<vector<RenderChunk>> Renderer::calculateVisibleRows(ScreenSize text_area_size) {
    Position first_visible = m_state.getFirstVisibleChar(text_area_size);

    int current_paragraph = first_visible.row;
    vector<vector<RenderChunk>> visible_rows;    
    visible_rows.reserve(text_area_size.height);

    bool is_first_paragraph = true;
    for (int visual_row = 0; visual_row < text_area_size.height;) {

        if (static_cast<size_t>(current_paragraph) >= m_state.getNumberOfParagrahps()) { 
            visible_rows.push_back({RenderChunk{
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

        for (const string& line : split) {
            if (visual_row < text_area_size.height) {
                visible_rows.push_back({RenderChunk{
                    line,
                    TextRole::TEXT_NORMAL
                }});
            }
            visual_row++;
        }
    
        current_paragraph++;
    }
    
    return visible_rows;
}

vector<RenderChunk> Renderer::calculateTemporaryRows(ScreenSize actual_size) {
    vector<RenderChunk> chunks;

    if (!m_state.getTemporaryMessages().empty()) {
        vector<RenderChunk> seperator_chunks = getSeperatorChunks(actual_size);
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