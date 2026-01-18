///
/// @file: TextFile.hpp
/// @description: description
///
/// @date: 2026-01-13
/// @author: Moritz Pirer
///

#ifndef TEXT_FILE_HPP
#define TEXT_FILE_HPP

#include <iostream>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "../Controller/Position.hpp"

class TextFile {
private:
    std::filesystem::path m_absolute_file_path;
    std::vector<std::string> m_file_content;
    bool m_has_unsaved_changes;
    size_t m_word_count;
    size_t m_character_count;

public:
    TextFile() = default;
    TextFile(const std::string& file_name);

    void setFilepath(std::filesystem::path new_absolute_file_path);
    const std::filesystem::path& getFilepath() const;
    void setHasUnsavedChanges(bool has_unsaved_changes);

    void writeToEnd(const std::string& line);
    
    int getLineCount() const;
    int getLineLength(size_t line_index) const;
    const std::string& getLine(size_t line_index) const;

    void calculateMetadata();

    static int visualLinesNeeded(int line_length, int screen_width);
    size_t visualLinesOfLine(size_t line_index, int screen_width) const;
};

#endif //TEXT_FILE_HPP
