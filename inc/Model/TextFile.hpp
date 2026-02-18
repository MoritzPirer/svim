///
/// @file: TextFile.hpp
/// @description: Internal representation of a file
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

#include "../Shared/Types/Position.hpp"
#include "SaveState.hpp"

class TextFile {
private:
    std::filesystem::path m_absolute_file_path;
    std::vector<std::string> m_file_content;
    
    SaveState m_save_state;

    size_t m_word_count;
    size_t m_character_count;

    /// @brief updates m_word_count and m_character_count
    void calculateMetadata();

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

    /// @brief inserts the given content into the file at the given position
    /// @param content a vector of paragraphs. the first and last entry will not be 
    ///     their own paragraphs, but rather joint with the remainder of the existing paragraph 
    /// @param start the position where the first character of content should be inserted
    void insertLines(std::vector<std::string> content, Position start);

    void setCharacterAt(char character_to_set, Position position);
    
    /// @return a copy of the content between start and end (inclusive)
    std::vector<std::string> copyRange(Position start, Position end);

    /// @brief deletes the content between start and end (inclusive)
    void deleteRange(Position start, Position end);
    
    /// @brief splits the paragraph containing the given position into two, with the given position
    ///     being the first character in the new paragraph
    /// @param first_of_new_paragraph where to split 
    void splitAt(Position first_of_new_paragraph);

    /// @brief appends the paragraph_index + 1-th paragraph to the paragraph_index-th
    /// @param paragraph_index the paragraph that absorbs its successor 
    void joinNextParagraphTo(int paragraph_index);
    
    int getNumberOfParagrahps() const;
    int getNumberOfWords() const;
    int getNumberofCharacters() const;

    int getParagraphLength(size_t index) const;
    const std::string& getParagraph(size_t index) const;

    static int visualLinesNeeded(int line_length, int screen_width);
    size_t visualLinesOfParagraph(size_t line_index, int screen_width) const;

    bool isValidCursorPosition(Position position);

    /// @brief like isValidCursorPosition, but excludes the overhang cursor position
    bool isValidTextPosition(Position position);

};

#endif //TEXT_FILE_HPP
