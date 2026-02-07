///
/// @file: ToolMode.hpp
/// @description: the mode that contains most of the editing shortcuts
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef TOOL_MODE_HPP
#define TOOL_MODE_HPP

#include "EditorMode.hpp"
#include "../Control/CommandParser.hpp"

class ToolMode: public EditorMode {
private:
    CommandParser m_command_parser;

    ParseResult parseMouseMovement(Position click_position,
        ScreenSize actual_size, ScreenSize text_area_size);

    ParseResult parseSpecialKey(SpecialKey key, ScreenSize text_area_size);
    ParseResult parseStandardInput(int input, ScreenSize text_area_size, const Settings& settings);
public:
    ToolMode() = default;
    ToolMode(const ToolMode&) = default;
    ~ToolMode() = default;

    /// @brief generates the next mode and the resulting action of the input.
    /// @param input the user input 
    /// @param size the current size of the writing area 
    /// @return the next mode and the action to execute 
    ParseResult parseInput(Input input, ScreenSize actual_size,
        ScreenSize text_area_size, const Settings& settings) override;

    std::string getModeLabel() const override { return " TOOL MODE "; }
};

#endif //TOOL_MODE_HPP
