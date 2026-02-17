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

    // all undoable actions must override this
    virtual void undo(EditorState& state) { (void) state; }; 

    // all undoable actions must override this
    virtual bool canBeUndone() const { return false; };
};

#endif //ACTION_HPP
