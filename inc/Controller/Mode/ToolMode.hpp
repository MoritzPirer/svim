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

    std::pair<ModeType, std::shared_ptr<Action>>  parseInput(int input) override;

    std::string getModeLabel() const override { return "TOOL"; }
};

#endif //TOOL_MODE_HPP
