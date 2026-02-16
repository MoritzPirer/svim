///
/// @file: TypingMode.hpp
/// @description: the mode that is primarily used for adding and deleting characters in the classic way
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef TYPING_MODE_HPP
#define TYPING_MODE_HPP

#include "EditorMode.hpp"

class TypingMode: public EditorMode {
private:
    ParseResult parseMouseMovement(Position click_position,
        ScreenSize actual_size, ScreenSize text_area_size);

    ParseResult parseSpecialKey(SpecialKey key, ScreenSize text_area_size, const Settings& settings, const EditorState& state);

public:
    TypingMode() = default;
    TypingMode(const TypingMode&) = default;
    ~TypingMode() = default;

    /// @brief generates the next mode and the resulting action of the input.
    /// @param input the user input 
    /// @param size the current size of the writing area 
    /// @return the next mode and the action to execute 
    ParseResult parseInput(Input input, ScreenSize actual_size,
        ScreenSize text_area_size, const Settings& settings, const EditorState& state) override;

    std::string getModeLabel() const override { return "TYPING MODE"; }
};

#endif //TYPING_MODE_HPP
