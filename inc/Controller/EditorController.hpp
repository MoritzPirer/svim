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

    size_t getCursorRow() const;
    size_t getCursorColumn() const;

    bool processInput(int input);
    
    size_t getLineCount() const;
    const std::string& getLine(size_t row) const;
};

#endif //EDITOR_CONTROLLER_HPP
