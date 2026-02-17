///
/// @file: IndentAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef INDENT_ACTION_HPP
#define INDENT_ACTION_HPP

#include "Action.hpp"

class IndentAction: public Action {
private:
    int m_row;
    int m_max_indent_width;
    int m_spaces_added;
public:
    IndentAction(int row, int max_indent_width);
    IndentAction(const IndentAction&) = default;
    ~IndentAction() = default;

    void apply(ExecutionContext& context) override;

    void undo(EditorState& state) override;

    bool canBeUndone() const override;
};

#endif //INDENT_ACTION_HPP
