#include <algorithm>
#include "../../inc/Model/TextFile.hpp"

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

size_t TextFile::getLineCount() const {
    return m_file_content.size();
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