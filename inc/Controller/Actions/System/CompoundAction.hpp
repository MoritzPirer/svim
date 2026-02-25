///
/// @file: CompoundAction.hpp
/// @description: An action that groups multiple other actions
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef COMPOUND_ACTION_HPP
#define COMPOUND_ACTION_HPP

#include <memory>
#include <vector>

#include "../Action.hpp"

class CompoundAction: public Action {
private:
    std::vector<std::shared_ptr<Action>> m_actions;
public:
    CompoundAction(std::vector<std::shared_ptr<Action>> actions);
    CompoundAction(const CompoundAction&) = default;
    ~CompoundAction() = default;

    void apply(ExecutionContext& context) override;

    bool canBeUndone() const override;

    void undo(EditorState& state) override;

};

#endif //COMPOUND_ACTION_HPP
