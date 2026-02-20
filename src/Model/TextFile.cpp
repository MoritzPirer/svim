#include "../../inc/Model/TextFile.hpp"
#include "../../inc/Shared/Utils/StringHelpers.hpp"

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

bool TextFile::isOverhangPosition(Position position) {
    if (static_cast<size_t>(position.row) < m_file_content.size()
        || static_cast<size_t>(position.column) == m_file_content.at(position.row).length()) {
            
        return true;
    }

    return false;
} 

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

void TextFile::insertLines(std::vector<std::string> content, Position start) {
    if (content.empty()) {
        return;
    }

    std::string& first_line = m_file_content.at(start.row);
    std::string rest_of_line = first_line.substr(start.column);
    first_line.erase(start.column);

    m_file_content.at(start.row).append(content.at(0));

    m_file_content.insert(m_file_content.begin() + start.row + 1, content.begin() + 1, content.end());
    m_file_content.at(start.row + content.size() - 1).append(rest_of_line);

    
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

std::vector<std::string> TextFile::copyRange(Position start, Position end) {
    if (!isValidCursorPosition(start) || !isValidCursorPosition(end)) {
        throw std::invalid_argument("copy start " + start.format()
            + " and / or end " + end.format() + " invalid!");
    }

    if (start.row > end.row
        || (start.row == end.row && start.column > end.column)) {
        throw std::invalid_argument("End before Start!");
    }

    // // IN AUSTRIA, WE CALL THIS 'PFUSCH'. FIX THIS PROPERLY SOON
    // if (!isValidTextPosition(start) || !isValidTextPosition(end)) {
    //     return {""};
    // }

    // std::vector<std::string> copy;
    // copy.reserve(end.row - start.row + 1);

    // for (int row = start.row; row <= end.row; row++) {
    //     std::string& current = m_file_content.at(row);
    //     int start_column = (row == start.row? start.column : 0);
    //     int end_column = (row == end.row? end.column : current.length() - 1);

    //     copy.emplace_back(current.substr(start_column, end_column - start_column + 1));
    // }

    // return copy;
    // if (!isValidCursorPosition(start) || !isValidCursorPosition(end)) {
    //     throw std::invalid_argument("Invalid copy range");
    // }

    // // 1. Normalize range
    // if (start.row > end.row || (start.row == end.row && start.column > end.column)) {
    //     std::swap(start, end);
    // }

    std::vector<std::string> result;

    // Case A: Single Line Copy
    if (start.row == end.row) {
        int row_len = (int)m_file_content.at(start.row).length();
        
        // If end is at the overhang, we captured the newline
        if (end.column >= row_len) {
            // Part 1: Text from start to end of string
            result.push_back(m_file_content.at(start.row).substr(start.column));
            // Part 2: An empty string to represent the "rest" of the line after the newline
            result.push_back(""); 
        } else {
            // Just a standard substring within the line
            int count = end.column - start.column + 1;
            result.push_back(m_file_content.at(start.row).substr(start.column, count));
        }
    } 
    // Case B: Multi-line Copy
    else {
        // 1. Start Line: from start.column to the end of that string
        result.push_back(m_file_content.at(start.row).substr(start.column));

        // 2. Intermediate Lines: full strings
        for (int i = start.row + 1; i < end.row; ++i) {
            result.push_back(m_file_content.at(i));
        }

        // 3. End Line: up to end.column (inclusive)
        int end_row_len = (int)m_file_content.at(end.row).length();
        if (end.column >= end_row_len) {
            // We included the overhang of the end line
            result.push_back(m_file_content.at(end.row));
            result.push_back(""); // Triggers the split in insertLines
        } else {
            result.push_back(m_file_content.at(end.row).substr(0, end.column + 1));
        }
    }

    return result;
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

// 2. Identify the "Suffix": content from the character AFTER the inclusive end
    std::string suffix = "";
    int suffix_row = end.row;
    int suffix_col = end.column + 1;

    // Check if the character after 'end' forces us to the next line
    if (suffix_col > (int)m_file_content.at(end.row).length()) {
        suffix_row++;
        suffix_col = 0;
    }

    // Capture the suffix if it's within file bounds
    if (suffix_row < (int)m_file_content.size()) {
        suffix = m_file_content.at(suffix_row).substr(suffix_col);
    }

    // 3. Update the start line
    // We keep the 'prefix' of the start row and attach the 'suffix' of the end row
    m_file_content.at(start.row) = m_file_content.at(start.row).substr(0, start.column) + suffix;

    // 4. Delete the engulfed lines
    // We remove every line index from (start.row + 1) up to suffix_row
    if (suffix_row > start.row) {
        auto erase_begin = m_file_content.begin() + start.row + 1;
        // We go to suffix_row + 1 because the line at suffix_row was merged into start.row
        auto erase_end = m_file_content.begin() + std::min((int)m_file_content.size(), suffix_row + 1);
        
        if (erase_begin < m_file_content.end() && erase_begin < erase_end) {
            m_file_content.erase(erase_begin, erase_end);
        }
    }

    // 5. Paragraph Cleanup
    // If the entire file was deleted, ensure one empty line remains.
    if (m_file_content.empty()) {
        m_file_content.push_back("");
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

void TextFile::joinNextParagraphTo(int paragraph_index) {
    if (paragraph_index < 0 || static_cast<size_t>(paragraph_index) >= m_file_content.size() - 1) {
        return;
    }

    m_file_content.at(paragraph_index).append(m_file_content.at(paragraph_index + 1));
    m_file_content.erase(m_file_content.begin() + paragraph_index + 1);
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

