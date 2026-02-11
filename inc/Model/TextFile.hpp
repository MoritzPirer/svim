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
#include "SaveState.hpp"

class TextFile {
private:
    std::filesystem::path m_absolute_file_path;
    std::vector<std::string> m_file_content;
    
    SaveState m_save_state;

    size_t m_word_count;
    size_t m_character_count;

public:
    TextFile() = default;
    TextFile(const std::string& file_name, SaveState save_state);
    TextFile(std::filesystem::path file_path, SaveState state);

    void setFilepath(std::filesystem::path new_absolute_file_path);
    const std::filesystem::path& getFilepath() const;

    void markAsSaved();

    /// @brief registers that changes have been made to the file and updates the save state
    ///     accordingly 
    void markAsChanged();
    SaveState getSaveState() const { return m_save_state; }

    /// @brief add a new line at the end of the file
    /// @param line the line to add
    void writeToEnd(const std::string& line);

    void insertCharacterAt(char character_to_add, Position position);
    void setCharacterAt(char character_to_set, Position position);
    
    void deleteRange(Position start, Position end);
    
    /// @brief splits the paragraph containing the given position into two, with the given position
    ///     being the first character in the new paragraph
    /// @param first_of_new_paragraph where to split 
    void splitAt(Position first_of_new_paragraph);

    /// @brief joins the paragraph with the given index to the previous paragraph
    /// @param paragraph_index index of the paragraph to join to the previous 
    void joinToPrevious(int paragraph_index);
    
    int getNumberOfParagrahps() const;
    int getNumberOfWords() const;
    int getNumberofCharacters() const;

    int getParagraphLength(size_t index) const;
    const std::string& getParagraph(size_t index) const;

    void calculateMetadata();

    static int visualLinesNeeded(int line_length, int screen_width);
    size_t visualLinesOfParagraph(size_t line_index, int screen_width) const;

    bool isValidCursorPosition(Position position);

    /// @brief like isValidCursorPosition, but excludes the overhang cursor position
    bool isValidTextPosition(Position position);

};

#endif //TEXT_FILE_HPP
