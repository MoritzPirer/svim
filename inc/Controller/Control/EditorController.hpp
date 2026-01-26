///
/// @file: EditorController.hpp
/// @description: description
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef EDITOR_CONTROLLER_HPP
#define EDITOR_CONTROLLER_HPP

#include <optional>

#include "../../Model/EditorState.hpp"
#include "../Mode/ModeManager.hpp"

class EditorController {
private:
    EditorState m_state;
    ModeManager m_mode_manager;
    
public:
    EditorController(std::optional<std::string> file_path);
    virtual ~EditorController() = default;


    int getCursorRow() const;
    int getCursorColumn() const;
    std::string getModeLabel() const;
    const std::string& getLine(size_t row) const;
    
    /// @return true if the program should end, false otherwise
    bool isQuit() const;

    /// @brief processes the given input with respet to the current mode and applies
    ///     any changes to the internal state of the file.
    /// @param input the input given by the user. Characters can be passed as-is, special characters
    ///     e.g. backspace, escape, should be passed using the constants defined in SpecialInputs.hpp 
    /// @param size the current size of the text area (needed for some actions, e.g. cursor movement) 
    void processInput(int input, ScreenSize size);

    /// @return returns the number of logical lines in the opened file
    int getLineCount() const;

    /// @brief calculates the position of the first character that should be visible on screen.
    ///     the calculation ensures that the cursor is always on the upper half of the screen
    /// @param size the current size of the text area 
    /// @return the position of the first visible character 
    Position getFirstVisibleChar(ScreenSize size);

    /// @param start the first character to be part of the partial line
    /// @return the rest of start's logical line (including start itself)
    std::string getPartialLine(Position start);

    /// returns the visual line the cursor should be rendered in (not necessarily the visual line
    ///     it actually is in)
    /// @param size the current size of te text area
    /// @return the visual lie of the cursor 
    int screenLineOfCursor(ScreenSize size);
};

#endif //EDITOR_CONTROLLER_HPP
