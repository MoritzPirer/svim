#include "../../inc/Model/TextFile.hpp"

TextFile::TextFile(std::string file_path):
    m_file_path{file_path},
    m_file_content{},
    m_word_count{0},
    m_character_count{0} {}

const std::string& TextFile::getFilePath() const {
    return m_file_path;
}

void TextFile::writeToEnd(const std::string& line) {
    m_file_content.emplace_back(line);
}

size_t TextFile::getLineCount() const {
    return m_file_content.size();
}
const std::string& TextFile::getLine(size_t line_index) const {
    //MODO bounds checking?
    return m_file_content.at(line_index); 
}

void TextFile::updateMetadata() {
    //recalculate char / word count
}