///
/// @file: EditorState.hpp
/// @description: Manages the internal state of the Editor, including the currently opened file and the
///     position of the cursor
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include <optional>

#include "../../inc/Model/TextFile.hpp"
#include "../../inc/Model/Cursor.hpp"
#include "../../inc/Shared/ScreenSize.hpp"
#include "../../inc/Shared/Direction.hpp"

class EditorState {
private:
    TextFile m_file;
    Cursor m_cursor;
    bool m_is_quit;

    std::vector<std::string> m_temporary_messages;

    bool isCursorInLastRowOfParagraph(int screen_width);
    
    /// @brief calculates the first visible char after skipping some number of visual rows
    /// @param offscreen_visual_lines how many lines to skip
    /// @param screen_width how wide the text area is
    Position skipOffscreenLines(int offscreen_visual_lines, int screen_width) const;

public:
    EditorState():
        m_file{},
        m_cursor{},  
        m_is_quit{false} {}

    EditorState(TextFile file): m_file{file} {}  
    
    EditorState(const EditorState&) = default;
    ~EditorState() = default;
    
    const Cursor& getCursor() const { return m_cursor; }
    TextFile& getFile() { return m_file; }
    bool getIsQuit() const { return m_is_quit; }
    void setIsQuit(bool is_quit) { m_is_quit = is_quit; } 
    
    void moveCursorUp(int screen_width);
    void moveCursorLeft();
    void moveCursorDown(int screen_width);
    void moveCursorRight();

    /// @param direction should be LEFT or RIGHT, otherwise the function has no effect
    void moveCursorSideways(Direction direction);
    void moveCursor(Direction direction, int screen_width);
    void moveCursorTo(Position position);
    bool canCursorMove(Direction direction) const;

    void insertCharacterAt(char character_to_add, Position position);

    inline void debug(std::string line) {m_file.writeToEnd(line); } 

    std::optional<char> readCharacterAt(Position position);
    std::optional<char> readCharacterAtCursor();
    void setCharacterAt(char character_to_set, Position position);
    
    /// @brief deletes from start to end, including both end points. if the range is 
    ///     an empty line, it is removed, not just cleared.
    /// @param start the inclusive start of deletion 
    /// @param end the inclusive end of deletion 
    void deleteRange(Position start, Position end);

    /// @brief splits the current line of the cursor in two, with the cursor position
    ///     being the first character in the new line
    void splitAtCursor();

    /// @brief joins the given line to the end of previous line
    /// @param line the index of the line to join to the previous 
    void joinLineToPrevious(int line);

    Position getFirstVisibleChar(ScreenSize size) const;
    
    /// @return the number of paragraphs in the file
    size_t getNumberOfParagrahps() const { return m_file.getNumberOfParagrahps(); }
    size_t getNumberOfWords() const { return m_file.getNumberOfWords(); }
    size_t getNumberOfCharacters() const { return m_file.getNumberofCharacters(); }
    SaveState getSaveState() const { return m_file.getSaveState(); }
    std::string getFileName() const { return m_file.getFilepath().filename(); }

    const std::string& getParagraph(size_t row) const { return m_file.getParagraph(row); }

    int calculateVisualLineOfCursor(int screen_width) const;

    void addTemporaryMessage(std::string message);
    const std::vector<std::string>& getTemporaryMessages() const;
    void clearTemporaryMessages();
};

#endif //EDITOR_STATE_HPP
