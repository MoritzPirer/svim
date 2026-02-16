///
/// @file: ScopeAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-11
/// @author: Moritz Pirer
///

#ifndef SCOPE_ACTION_HPP
#define SCOPE_ACTION_HPP

#include "Action.hpp"
#include "ActionOptions/EndBehavior.hpp"
#include "../../Shared/Scope.hpp"
#include "../../Shared/Direction.hpp"

class ScopeAction: public Action {
private:
    ScreenSize m_size;
    EndBehavior m_end_behavior;

    Position startOfParagraph(const EditorState& state);
    Position startOfLine(const EditorState& state);

    Position endOfParagraph(const EditorState& state);
    Position endOfLine(const EditorState& state);

protected:
    Position startOfScope(const EditorState& state, Scope scope);
    Position endOfScope(const EditorState& state, Scope scope);

public:
    ScopeAction(
        ScreenSize size,
        EndBehavior end_behavior
    );
    ScopeAction(const ScopeAction&) = default;
    ~ScopeAction() = default;

    void apply(ExecutionContext& context) override = 0;
};

#endif //SCOPE_ACTION_HPP