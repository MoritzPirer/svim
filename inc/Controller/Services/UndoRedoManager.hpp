///
/// @file: UndoRedoManager.hpp
/// @description: handles the undo and redo history of the editor
///
/// @date: 2026-02-15
/// @author: Moritz Pirer
///

#ifndef UNDO_REDO_MANAGER_HPP
#define UNDO_REDO_MANAGER_HPP

#include <memory>
#include <deque>
#include <stack>

class Action;
class EditorState;

class UndoRedoManager {
private:
    std::deque<std::shared_ptr<Action>> m_undoable_actions;
    std::stack<std::shared_ptr<Action>> m_redoable_actions;
    static constexpr int c_max_history_size = 100;

public:
    UndoRedoManager() = default;
    UndoRedoManager(const UndoRedoManager&) = default;
    ~UndoRedoManager() = default;

    void add(std::shared_ptr<Action> action);

    void undo(EditorState& state);

    void redo(EditorState& state);
};

#endif //UNDO_REDO_MANAGER_HPP
