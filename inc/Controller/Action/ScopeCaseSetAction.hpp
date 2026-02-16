///
/// @file: ScopeCaseSetAction.hpp
/// @description: sets the case for all characters in a scope
///
/// @date: 2026-02-11
/// @author: Moritz Pirer
///

#ifndef SCOPE_CASE_SET_ACTION_HPP
#define SCOPE_CASE_SET_ACTION_HPP

#include "ScopeAction.hpp"
#include "ActionOptions/EndBehavior.hpp"
#include "ActionOptions/Case.hpp"
#include "../../Shared/Scope.hpp"
#include "../../Shared/Direction.hpp"

class ScopeCaseSetAction: public ScopeAction {
private:
    Scope m_scope;
    Case m_target_case;
    
public:
    ScopeCaseSetAction(
        ScreenSize size,
        Scope scope,
        Case target_case
    );
    ScopeCaseSetAction(const ScopeCaseSetAction&) = default;
    ~ScopeCaseSetAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //SCOPE_CASE_SET_ACTION_HPP