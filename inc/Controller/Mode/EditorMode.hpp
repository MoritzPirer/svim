///
/// @file: EditorMode.hpp
/// @description: abstract template for a mode the editor can be in
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef EDITOR_MODE_HPP
#define EDITOR_MODE_HPP

#include <string>
#include <memory>
#include <vector>

#include "../Action/Action.hpp"
#include "../../Shared/ParseResult.hpp"
#include "../Settings/Settings.hpp"
#include "../../Shared/Input.hpp"

class EditorMode {
public:
    EditorMode() = default;
    EditorMode(const EditorMode&) = default;
    virtual ~EditorMode() = default;

    /// @brief parses the input and converts it into an editor mode and zero or more Actions
    /// @param input the input to process 
    /// @param size the current size of the screen area
    /// @return the mode the editor should transition to (may be the same as currently), one or more
    ///     actions to apply to the editor state (which may be empty if nothing should be done) 
    virtual ParseResult parseInput(Input input, ScreenSize screen_size,
        ScreenSize text_area_size, const Settings& settings, const EditorState& state) = 0;

    /// @brief returns the name of the mode for displaying
    virtual std::string getModeLabel() const = 0;
};

#endif //EDITOR_MODE_HPP
