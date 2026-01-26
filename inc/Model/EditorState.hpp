///
/// @file: EditorState.hpp
/// @description: description
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include "../../inc/Model/TextFile.hpp"
#include "../../inc/Model/Cursor.hpp"
#include "../../inc/Controller/ScreenSize.hpp"

class EditorState {
private:
    TextFile m_file;
    Cursor m_cursor;
    bool m_is_quit;

    bool isLastVisualLineOfLine(int screen_width);
    
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


    void insertCharacterAtCursor(char character_to_add);
    
    /// @brief deletes from start to end, including both end points. if the range is 
    ///     an empty line, it is remnoved, not just cleared.
    /// @param start the inclusive start of deletion 
    /// @param end the inclusive end of deletion 
    void deleteRange(Position start, Position end);

    /// @brief splits the current line of the cursor in two, with the cursor position
    ///     being the first character in the new line
    void splitAtCursor();

    /// @brief joins the given line to the end of previous line
    /// @param line the index of the line to join to the previous 
    void joinLineToPrevious(int line);

    
    Position getFirstVisibleChar(ScreenSize size);
    
    size_t getLineCount() const { return m_file.getLineCount(); }
    
    std::string getPartialLine(Position start);
    const std::string& getLine(size_t row) const { return m_file.getLine(row); }
    int calculateVisualLineOfCursor(int screen_width) const;
};

#endif //EDITOR_STATE_HPP
