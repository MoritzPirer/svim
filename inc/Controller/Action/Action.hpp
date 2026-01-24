///
/// @file: Action.hpp
/// @description: describes an action that can be performed by the editor
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef ACTION_HPP
#define ACTION_HPP

#include "../../Model/EditorState.hpp"
#include "../ScreenSize.hpp"

class Action {
public:
    Action() = default;
    Action(const Action&) = default;
    virtual ~Action() = default;

    virtual void applyTo(EditorState& state, ScreenSize size) = 0;
};

#endif //ACTION_HPP
