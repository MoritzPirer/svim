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

#include "../../inc/Model/FileHandler.hpp"

class EditorState {
private:
    TextFile m_file;
    Cursor m_cursor;
    
    bool isLastVisualLineOfLine(int screen_width);
public:
    EditorState() {
        FileHandler f;
        m_file = f.openFile("test.txt");
    }

    EditorState(const EditorState&) = default;
    ~EditorState() = default;

    const Cursor& getCursor() const { return m_cursor; }

    void moveCursorUp(int screen_width);
    void moveCursorLeft();
    void moveCursorDown(int screen_width);
    void moveCursorRight();

    size_t getFirstVisibleLine(int screen_width, int screen_height);
    size_t getLineCount() const { return m_file.getLineCount(); }
    const std::string& getLine(size_t row) const { return m_file.getLine(row); }
};

#endif //EDITOR_STATE_HPP
