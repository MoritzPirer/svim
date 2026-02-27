#include "../../../inc/Controller/Services/UndoRedoManager.hpp"
#include "../../../inc/Controller/Actions/Action.hpp"
#include "../../../inc/Controller/Actions/ExecutionContext.hpp"

using std::shared_ptr;

void UndoRedoManager::add(shared_ptr<Action> action) {
    if (!action->canBeUndone()) {
        m_merge_possible = false;
        return;
    }

    if (m_merge_possible && !m_undoable_actions.empty()
        && m_undoable_actions.back()->canAbsorb(action)) {

        m_undoable_actions.back()->absorb(action);
    }
    else {
        m_merge_possible = true;
        m_undoable_actions.push_back(action);
        m_redoable_actions = std::stack<shared_ptr<Action>>();
    }


    if (m_undoable_actions.size() > c_max_history_size) {
        m_undoable_actions.pop_front();
    }
}

void UndoRedoManager::undo(EditorState& state) {
    if (m_undoable_actions.empty()) {
        state.addTemporaryMessage("No actions to undo!");
        return;
    }

    
    shared_ptr<Action> action = m_undoable_actions.back();
    m_undoable_actions.pop_back(); 
    
    action->undo(state);

    m_redoable_actions.push(action);
}

void UndoRedoManager::redo(ExecutionContext& context) {
    if (m_redoable_actions.empty()) {
        context.state.addTemporaryMessage("No actions to redo!");
        return;
    }
    
    shared_ptr<Action> action = m_redoable_actions.top();
    m_redoable_actions.pop();

    action->apply(context);

    m_undoable_actions.push_back(action);
}