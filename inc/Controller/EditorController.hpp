///
/// @file: EditorController.hpp
/// @description: description
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef EDITOR_CONTROLLER_HPP
#define EDITOR_CONTROLLER_HPP

#include "../Model/EditorState.hpp"

class EditorController {
private:
    EditorState m_state;
public:
    EditorController() = default;
    EditorController(const EditorController&) = default;
    ~EditorController() = default;

    int getCursorRow() const;
    int getCursorColumn() const;

    bool processInput(int input, int screen_width);
    
    int getLineCount() const;
    Position getFirstVisibleChar(int screen_width, int screen_height);
    std::string getPartialLine(Position start);
    const std::string& getLine(size_t row) const;
};

#endif //EDITOR_CONTROLLER_HPP
