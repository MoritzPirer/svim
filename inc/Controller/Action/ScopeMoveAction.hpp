///
/// @file: ScopeMoveAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef SCOPE_MOVE_ACTION_HPP
#define SCOPE_MOVE_ACTION_HPP

#include "Action.hpp"
#include "ActionOptions/ActionDirection.hpp"
#include "ActionOptions/EndBehavior.hpp"
#include "../../Shared/Scope.hpp"

class ScopeMoveAction: public Action {
private:
    ScreenSize m_size;
    Scope m_scope;
    Direction m_move_direction;
    EndBehavior m_end_behavior;

    void fileScopeMove(EditorState& state);
    void paragraphScopeMove(EditorState& state);
    void lineScopeMove(EditorState& state);
public:
    ScopeMoveAction(
        ScreenSize size,
        Scope scope,
        Direction move_direction,
        EndBehavior end_behavior
    );
    ScopeMoveAction(const ScopeMoveAction&) = default;
    ~ScopeMoveAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //SCOPE_MOVE_ACTION_HPP