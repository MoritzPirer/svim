///
/// @file: ScopeMoveAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef SCOPE_MOVE_ACTION_HPP
#define SCOPE_MOVE_ACTION_HPP

#include "ScopeAction.hpp"
#include "ActionOptions/EndBehavior.hpp"
#include "../../Shared/Scope.hpp"
#include "../../Shared/Direction.hpp"

class ScopeMoveAction: public ScopeAction {
private:
    Scope m_scope;
    Direction m_move_direction;

public:
    ScopeMoveAction(
        ScreenSize size,
        EndBehavior end_behavior,
        Scope scope,
        Direction move_direction
    );
    ScopeMoveAction(const ScopeMoveAction&) = default;
    ~ScopeMoveAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //SCOPE_MOVE_ACTION_HPP