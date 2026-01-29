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

#include "../Shared/Position.hpp"

class TextFile {
private:
    std::filesystem::path m_absolute_file_path;
    std::vector<std::string> m_file_content;
    bool m_has_unsaved_changes;
    size_t m_word_count;
    size_t m_character_count;

    void validatePosition(Position position);

public:
    TextFile() = default;
    TextFile(const std::string& file_name);
    TextFile(std::filesystem::path file_path);

    void setFilepath(std::filesystem::path new_absolute_file_path);
    const std::filesystem::path& getFilepath() const;
    void setHasUnsavedChanges(bool has_unsaved_changes);

    /// @brief add a new line at the end of the file
    /// @param line the line to add
    void writeToEnd(const std::string& line);

    void insertCharacterAt(char character_to_add, Position position);
    
    void deleteRange(Position start, Position end);
    void splitAt(Position first_of_new_paragraph);
    void joinToPrevious(int paragraph_index);
    
    int getNumberOfParagrahps() const;
    int getLineLength(size_t line_index) const;
    const std::string& getLine(size_t line_index) const;

    void calculateMetadata();

    static int visualLinesNeeded(int line_length, int screen_width);
    size_t visualLinesOfParagraph(size_t line_index, int screen_width) const;
};

#endif //TEXT_FILE_HPP
