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
#include "../../Shared/ScreenSize.hpp"

struct ExecutionContext;

class Action {
public:
    Action() = default;
    Action(const Action&) = default;
    virtual ~Action() = default;

    virtual void apply(ExecutionContext& context) = 0;

    virtual void undo(EditorState& state) { (void) state; }; // temporary; will be replaced by pure virtual

    virtual bool canBeUndone() const { return false; }; // temporary; will be pure virtual
};

#endif //ACTION_HPP
