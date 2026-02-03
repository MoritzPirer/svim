#include "../../../inc/Controller/Control/EditorController.hpp"
#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Shared/StringHelpers.hpp"

EditorController::EditorController(std::optional<std::string> file_path):
    m_mode_manager{ModeType::TOOL_MODE} {

    TextFile file;
    if (!file_path.has_value() || file_path->empty()) {
        std::filesystem::path name = FileHandler::getDefaultName(); 
        file = FileHandler::createFile(name);
    }
    else {
        file = FileHandler::openFile(file_path.value());      
    }
    m_state = EditorState(file);
}

Position EditorController::getFirstVisibleChar(ScreenSize size) {
    return m_state.getFirstVisibleChar(size);
}

std::vector<std::string> EditorController::splitIntoRows(const std::string& paragraph,
    int start_column, int max_length) const {

    if (paragraph.length() == 0) {
        return {""};
    }

    std::vector<std::string> split;
    //split.reserve((paragraph.length() - start_column) / width);

    for (int i = start_column; static_cast<size_t>(i) < paragraph.length(); i += max_length) {
        split.emplace_back(paragraph.substr(i, max_length));
    }

    return split;
}

std::vector<std::string> EditorController::calculateVisibleRows(ScreenSize text_area_size) {
    Position first_visible = getFirstVisibleChar(text_area_size);

    int current_paragraph = first_visible.row;
    std::vector<std::string> visible_rows;    
    visible_rows.reserve(text_area_size.height);

    bool is_first_paragraph = true;
    for (int visual_row = 0; visual_row < text_area_size.height;) {

        if (static_cast<size_t>(current_paragraph) < m_state.getNumberOfParagrahps()) { 
            
            std::vector<std::string> split = splitIntoRows(
                m_state.getParagraph(current_paragraph),
                (is_first_paragraph? first_visible.column : 0),
                text_area_size.width 
            );
            is_first_paragraph = false;


            for (const std::string& line : split) {
                if (visual_row < text_area_size.height) {
                    visible_rows.emplace_back(line);
                }

                visual_row++;
            }
        
            current_paragraph++;
        }        
        else {
            visible_rows.emplace_back("~"); // FUTURE: load placeholder line from settings
            visual_row++;
        }
    }
    
    return visible_rows;
}

Position EditorController::calculateScreenPositionOfCursor(ScreenSize text_area_size) {
    int screen_row = std::min(
        text_area_size.height / 2,
        m_state.calculateVisualLineOfCursor(text_area_size.width)
    ); 

    int screen_column = m_state.getCursor().getColumn() % text_area_size.width;

    return {screen_row, screen_column};
}

std::vector<RenderChunk> EditorController::getSeperatorChunks(ScreenSize actual_size) {
    bool show_seperator = true;

    return { RenderChunk{
        std::string(actual_size.width - 1, (show_seperator ? '-' : ' ')),
        TextRole::UI_ELEMENT
    }};
}

std::vector<RenderChunk> EditorController::getTemporaryMessageChunks(ScreenSize actual_size) {
    std::vector<RenderChunk> chunks;

    bool has_message = false;
    for (const std::string& message : m_state.getTemporaryMessages()) {
        for (const std::string& message_row : splitIntoRows(message, 0, actual_size.width - 1)) {
            chunks.push_back({
                StringHelpers::leftAlign(message_row, actual_size.width),
                TextRole::TEXT_HIGHLIGHT
            });
        }
        
        if (message.length() > 0) {
            has_message = true;
        }
    }

    if (has_message == true) {
        std::vector<RenderChunk> seperator_chunks = getSeperatorChunks(actual_size);
        chunks.insert(chunks.end(), seperator_chunks.begin(), seperator_chunks.end());
    }

    return chunks;
}

std::vector<RenderChunk> EditorController::getCharacterCountChunks() {
    bool show_character_count = true;
    if (!show_character_count) {
        return {};
    }

    int character_count = m_state.getNumberOfCharacters();
    return {
        {"Character(s): ", TextRole::TEXT_NORMAL},
        {std::to_string(character_count), TextRole::TEXT_HIGHLIGHT}
    };
}

std::vector<RenderChunk> EditorController::getWordCountChunks() {
    bool show_word_count = true;
    if (!show_word_count) {
        return {};
    }

    int word_count = m_state.getNumberOfWords();
    return {
        {"Word(s): ", TextRole::TEXT_NORMAL},
        {std::to_string(word_count), TextRole::TEXT_HIGHLIGHT}
    };
}

std::vector<RenderChunk> EditorController::getParagraphCountChunks() {
    bool show_paragraph_count = true;
    if (!show_paragraph_count) {
        return {};
    }

    int paragraph_count = m_state.getNumberOfParagrahps();
    return {
        {"Paragraph(s): ", TextRole::TEXT_NORMAL},
        {std::to_string(paragraph_count), TextRole::TEXT_HIGHLIGHT}
    };
}

std::vector<RenderChunk> EditorController::getCursorPositionChunks() {
    bool show_cursor_position = true;

    if (!show_cursor_position) {
        return {};
    }

    return {
        {"Cursor position: ", TextRole::TEXT_NORMAL},
        {m_state.getCursor().getPosition().format(), TextRole::TEXT_HIGHLIGHT}
    };
}

std::vector<RenderChunk> EditorController::getFileNameChunks() {
    bool show_file_name = true;

    if (!show_file_name) {
        return {};
    }

    return {{"Currently editing " + m_state.getFileName(), TextRole::TEXT_NORMAL}};
}

std::vector<RenderChunk> EditorController::getSaveIconChunks() {
    bool show_save_icon = true;
    bool show_save_text = true;

    if (!show_save_icon && !show_save_text) {
        return {};
    }
    
    std::string icon = "";
    std::string text = "";
    TextRole role;    

    switch (m_state.getSaveState()) {
        case SaveState::NEW_FILE: {
            icon = "[!]";
            text = "New File";
            role = TextRole::FILE_NEW;
            break;
        };
        case SaveState::UNSAVED_CHANGES: {
            icon = "[*]";
            text = "Unsaved Changes";
            role = TextRole::FILE_CHANGED;
            break;
        };
        case SaveState::SAVED: {
            icon = "[=]";
            text = "Changes Saved";
            role = TextRole::FILE_SAVED;
        };
    }

    std::string message = icon + (show_save_icon && show_save_text? " " : "") + text;

    return {{message, role}};
}

std::vector<RenderChunk> EditorController::getVersionChunks() {
    bool show_version = true;
    if (!show_version) {
        return {};
    }

    return {{getVersion(), TextRole::TEXT_NORMAL}};
}


std::vector<RenderChunk> EditorController::getEditorModeChunks() {
    bool show_editor_mode = true;
    if (!show_editor_mode) {
        return {};
    }

    return {{m_mode_manager.getModeLabel(), TextRole::TEXT_HIGHLIGHT}};
}

std::vector<std::vector<RenderChunk>> EditorController::reorganizeMetadataRows(
    std::vector<RenderChunk> ordered_chunks, ScreenSize actual_size) {
    
    int row_length = 0;
    int chunks_in_row = 0;
    std::vector<std::vector<RenderChunk>> rows;
    std::vector<RenderChunk> current_row;

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

std::vector<std::vector<RenderChunk>> EditorController::calculateMetadataRows(ScreenSize actual_size) {
    std::vector<RenderChunk> ordered_chunks; 

    auto addContent = [&](const std::vector<RenderChunk>& chunks, bool supress_divider = false) {
        ordered_chunks.insert(ordered_chunks.end(), chunks.begin(), chunks.end());
        if (chunks.size() != 0 && !supress_divider) {
            ordered_chunks.push_back({" | ", TextRole::TEXT_NORMAL});
        }
    };
    
    addContent(getSeperatorChunks(actual_size), true);
    addContent(getTemporaryMessageChunks(actual_size), true);
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

RenderInfo EditorController::calculateRenderInfo(ScreenSize actual_size) {
    std::vector<std::vector<RenderChunk>> metadata_rows = calculateMetadataRows(actual_size);
    ScreenSize text_area_size = actual_size; //FUTURE: also account for line number width
    text_area_size.height -= metadata_rows.size();

    bool should_render_color = true;

    return {
        calculateVisibleRows(text_area_size),
        metadata_rows,
        calculateScreenPositionOfCursor(text_area_size),
        should_render_color
    };
}

void EditorController::mainLoop() {
    while (m_state.getIsQuit() == false) {
        RenderInfo render_info = calculateRenderInfo(m_ui_handler.screenSize());
        m_ui_handler.render(render_info);
        int input = m_ui_handler.getInput();

        m_state.clearTemporaryMessages();
        std::vector<std::shared_ptr<Action>> actions = m_mode_manager.convertToAction(
            input,
            m_ui_handler.screenSize()
        );

        for (std::shared_ptr action : actions) {
            action->applyTo(m_state);
        } 
    }
}