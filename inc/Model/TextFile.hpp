///
/// @file: TextFile.hpp
/// @description: description
///
/// @date: 2026-01-13
/// @author: Moritz Pirer
///

#ifndef TEXT_FILE_HPP
#define TEXT_FILE_HPP

#include <fstream>
#include <string>
#include <vector>

class TextFile {
private:
    std::string m_file_path;
    std::vector<std::string> m_file_content;
    size_t m_word_count;
    size_t m_character_count;

public:
    TextFile(std::string file_path);

    const std::string& getFilePath() const;

    void writeToEnd(const std::string& line);
    
    size_t getLineCount() const;
    const std::string& getLine(size_t line_index) const;

    void updateMetadata();
};

#endif //TEXT_FILE_HPP
