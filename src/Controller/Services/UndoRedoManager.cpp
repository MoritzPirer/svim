#include "../../../inc/Controller/Services/UndoRedoManager.hpp"
#include "../../../inc/Controller/Actions/Action.hpp"

using std::shared_ptr;

void UndoRedoManager::add(shared_ptr<Action> action) {
    if (!action->canBeUndone()) {
        return;
    }

    m_undoable_actions.push_back(action);
    m_redoable_actions = std::stack<shared_ptr<Action>>();

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