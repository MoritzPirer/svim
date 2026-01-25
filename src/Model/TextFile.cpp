#include <algorithm>
#include "../../inc/Model/TextFile.hpp"

TextFile::TextFile(std::filesystem::path file_path):
    m_absolute_file_path{file_path},
    m_file_content{},
    m_has_unsaved_changes{false},
    m_word_count{0},
    m_character_count{0} {}

TextFile::TextFile(const std::string& file_path):
    m_absolute_file_path{std::filesystem::absolute(file_path)},
    m_file_content{},
    m_has_unsaved_changes{false},
    m_word_count{0},
    m_character_count{0} {}

void TextFile::setFilepath(std::filesystem::path new_absolute_file_path) {
    m_absolute_file_path = new_absolute_file_path;
}

const std::filesystem::path& TextFile::getFilepath() const {
    return m_absolute_file_path;
}

void TextFile::setHasUnsavedChanges(bool has_unsaved_changes) {
    m_has_unsaved_changes = has_unsaved_changes;
}

void TextFile::writeToEnd(const std::string& line) {
    m_file_content.emplace_back(line);
    m_has_unsaved_changes = true;
    
    calculateMetadata();
}

void TextFile::insertCharacterAt(char character_to_add, Position position) {
    if (static_cast<size_t>(position.row) >= m_file_content.size()
        || static_cast<size_t>(position.column) > m_file_content.at(position.row).length()) {
            
        throw std::out_of_range("Cannot insert at position" + position.format());
    }

    std::string& line = m_file_content.at(position.row);
    line.insert(line.begin() + position.column, character_to_add);
}

int TextFile::getLineCount() const {
    return m_file_content.size();
}

int TextFile::getLineLength(size_t line_index) const {
    return m_file_content.at(line_index).length();
}

const std::string& TextFile::getLine(size_t line_index) const {
    //MODO bounds checking?
    return m_file_content.at(line_index); 
}

void TextFile::calculateMetadata() {
    //recalculate char / word count
    m_character_count = 0;
    m_word_count = 0;

    for (const std::string& line : m_file_content) {
        m_character_count += line.length();
        m_word_count += std::ranges::count(line, ' ');
    }
}

int TextFile::visualLinesNeeded(int line_length, int screen_width) {
    if (line_length == 0) return 1;
                
    bool has_partial_line = line_length % screen_width != 0;

    return line_length / screen_width + (has_partial_line? 1 : 0);
}

size_t TextFile::visualLinesOfLine(size_t line_index, int screen_width) const {
    size_t line_length = m_file_content.at(line_index).length();
    return visualLinesNeeded(line_length, screen_width);
}

