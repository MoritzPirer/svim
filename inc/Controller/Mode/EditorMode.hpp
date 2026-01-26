///
/// @file: EditorMode.hpp
/// @description: description
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef EDITOR_MODE_HPP
#define EDITOR_MODE_HPP

#include <string>
#include <memory>

#include "../Action/Action.hpp"
#include "ModeType.hpp"

class EditorMode {
private:
    
public:
    EditorMode() = default;
    EditorMode(const EditorMode&) = default;
    virtual ~EditorMode() = default;

    virtual std::pair<ModeType, std::shared_ptr<Action>> parseInput(int input, ScreenSize size) = 0;

    virtual std::string getModeLabel() const = 0;
};

#endif //EDITOR_MODE_HPP
