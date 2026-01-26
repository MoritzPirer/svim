///
/// @file: ToolMode.hpp
/// @description: description
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef TOOL_MODE_HPP
#define TOOL_MODE_HPP

#include "EditorMode.hpp"

class ToolMode: public EditorMode {
private:

public:
    ToolMode() = default;
    ToolMode(const ToolMode&) = default;
    ~ToolMode() = default;

    /// @brief generates the next mode and the resulting action of the input.
    /// @param input the user input 
    /// @param size the current size of the writing area 
    /// @return the next mode and the action to execute 
    std::pair<ModeType, std::shared_ptr<Action>> parseInput(int input, ScreenSize size) override;

    std::string getModeLabel() const override { return "TOOL"; }
};

#endif //TOOL_MODE_HPP
