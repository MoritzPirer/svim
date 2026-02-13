#include <algorithm>
#include "../../inc/Model/TextFile.hpp"
#include "../../inc/Shared/StringHelpers.hpp"

TextFile::TextFile(std::filesystem::path file_path, SaveState save_state):
    m_absolute_file_path{file_path},
    m_file_content{},
    m_save_state{save_state},
    m_word_count{0},
    m_character_count{0} {}

TextFile::TextFile(const std::string& file_path, SaveState save_state):
    m_absolute_file_path{std::filesystem::absolute(file_path)},
    m_file_content{},
    m_save_state{save_state},
    m_word_count{0},
    m_character_count{0} {}

bool TextFile::isValidCursorPosition(Position position) {
    if (static_cast<size_t>(position.row) >= m_file_content.size()
        || static_cast<size_t>(position.column) > m_file_content.at(position.row).length()) {
            
        return false;
    }

    return true;
}

bool TextFile::isValidTextPosition(Position position) {
    if (static_cast<size_t>(position.row) >= m_file_content.size()
        || static_cast<size_t>(position.column) >= m_file_content.at(position.row).length()) {
            
        return false;
    }

    return true;
}


void TextFile::setFilepath(std::filesystem::path new_absolute_file_path) {
    m_absolute_file_path = new_absolute_file_path;
}

const std::filesystem::path& TextFile::getFilepath() const {
    return m_absolute_file_path;
}

void TextFile::markAsSaved() {
    m_save_state = SaveState::SAVED;
}

void TextFile::markAsChanged() {
    // NEW_FILE is stronger than UNSAVED_CHANGES
    if (m_save_state != SaveState::NEW_FILE) {
        m_save_state = SaveState::UNSAVED_CHANGES;
    }
}


void TextFile::writeToEnd(const std::string& line) {
    m_file_content.emplace_back(line);
    
    if (m_save_state != SaveState::NEW_FILE) {
        m_save_state = SaveState::UNSAVED_CHANGES;
    }
    
    calculateMetadata();
}

void TextFile::insertCharacterAt(char character_to_add, Position position) {
    if (!isValidCursorPosition(position)) {
        throw std::invalid_argument("Insertion attempted at invalid position " + position.format() + "!");
    }

    std::string& line = m_file_content.at(position.row);
    line.insert(line.begin() + position.column, character_to_add);
    
    markAsChanged();
    calculateMetadata();
}

void TextFile::setCharacterAt(char character_to_set, Position position) {
    if (!isValidTextPosition(position)) {
        throw std::invalid_argument("Set attempted at invalid position " + position.format() + "!");
    }

    m_file_content.at(position.row).at(position.column) = character_to_set;
    
    markAsChanged();
    calculateMetadata();
}

void TextFile::deleteRange(Position start, Position end) {
    if (!isValidCursorPosition(start) || !isValidCursorPosition(end)) {
        throw std::invalid_argument("deletion start " + start.format()
            + " and / or end " + end.format() + " invalid!");
    }

    if (start.row > end.row
        || (start.row == end.row && start.column > end.column)) {
        throw std::invalid_argument("End before Start!");
    }

    // delete empty line (update logic for full line delete)
    if (start.row == end.row 
        && m_file_content.at(start.row).length() == 0) {
        
        if (m_file_content.size() > 1) { // always leave at least one empty line
            m_file_content.erase(m_file_content.begin() + start.row);
        } 
        
        markAsChanged();
        calculateMetadata();

        return;
    }

    for (int row = start.row; row <= end.row; row++) {
        if (row != start.row && row != end.row) { // entire line deleted
            m_file_content.erase(m_file_content.begin() + row);
            continue;
        }
        // partial line deleted
        std::string& line = m_file_content.at(row);
        int start_of_erase = (row == start.row? start.column : 0);
        int end_of_erase = (row == end.row? end.column : line.length()) + 1; 

        line.erase(line.begin() + start_of_erase, line.begin() + end_of_erase);
    }

    markAsChanged();
    calculateMetadata();
}

void TextFile::splitAt(Position first_of_new_paragraph) {
    if(!isValidCursorPosition(first_of_new_paragraph)) {
        throw std::invalid_argument("invalid splitting position " + first_of_new_paragraph.format() + "!");
    }

    std::string& line_to_split = m_file_content.at(first_of_new_paragraph.row);
    std::string split_line = line_to_split.substr(first_of_new_paragraph.column);
    line_to_split.erase(line_to_split.begin() + first_of_new_paragraph.column, line_to_split.end());
    m_file_content.insert(m_file_content.begin() + first_of_new_paragraph.row + 1, split_line);
    
    markAsChanged();
}

void TextFile::joinToPrevious(int paragraph_index) {
    if (paragraph_index == 0 || static_cast<size_t>(paragraph_index) >= m_file_content.size()) {
        return;
    }

    m_file_content.at(paragraph_index - 1).append(m_file_content.at(paragraph_index));
    m_file_content.erase(m_file_content.begin() + paragraph_index);

    markAsChanged();
}


int TextFile::getNumberOfParagrahps() const {
    return m_file_content.size();
}

int TextFile::getNumberOfWords() const {
    return m_word_count;
}

int TextFile::getNumberofCharacters() const {
    return m_character_count;
}

int TextFile::getParagraphLength(size_t index) const {
    return m_file_content.at(index).length();
}

const std::string& TextFile::getParagraph(size_t index) const {
    //MODO bounds checking?
    return m_file_content.at(index); 
}

void TextFile::calculateMetadata() {
    m_character_count = 0;
    m_word_count = 0;

    for (const std::string& line : m_file_content) {
        m_character_count += line.length();
        m_word_count += StringHelpers::countWords(line);
    }
}

int TextFile::visualLinesNeeded(int line_length, int screen_width) {
    if (line_length == 0) return 1;
                
    bool has_partial_line = line_length % screen_width != 0;

    return line_length / screen_width + (has_partial_line? 1 : 0);
}

size_t TextFile::visualLinesOfParagraph(size_t line_index, int screen_width) const {
    size_t line_length = m_file_content.at(line_index).length();
    return visualLinesNeeded(line_length, screen_width);
}

