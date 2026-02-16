///
/// @file: History.hpp
/// @description: description
///
/// @date: 2026-02-15
/// @author: Moritz Pirer
///

#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <memory>
#include <deque>
#include <stack>

class Action;
class EditorState;

class History {
private:
    std::deque<std::shared_ptr<Action>> m_undoable_actions;
    std::stack<std::shared_ptr<Action>> m_redoable_actions;
static constexpr int c_max_history_size = 100;public:
    History() = default;
    History(const History&) = default;
    ~History() = default;

    void add(std::shared_ptr<Action> action);

    void undo(EditorState& state);

    void redo(EditorState& state);
};

#endif //HISTORY_HPP
