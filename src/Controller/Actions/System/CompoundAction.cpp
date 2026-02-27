#include "../../../../inc/Controller/Actions/System/CompoundAction.hpp"

CompoundAction::CompoundAction(std::vector<std::shared_ptr<Action>> actions):
    m_actions{actions}
    {}

void CompoundAction::apply(ExecutionContext& context) {
    for (auto& action : m_actions) {
        action->apply(context);
    }
}

void CompoundAction::undo(EditorState& state) {
    for (auto action = m_actions.rbegin(); action != m_actions.rend(); action++) {
        (*action)->undo(state);
    }
}

bool CompoundAction::canBeUndone() const {
    for (auto action : m_actions) {
        if (action->canBeUndone()) {
            return true;
        }
    }
    
    return false;
}