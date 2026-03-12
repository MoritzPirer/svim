///
/// @file: UnindentAction.hpp
/// @description: reduces the indentation of a paragraph
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef UNINDENT_ACTION_HPP
#define UNINDENT_ACTION_HPP

#include "../Action.hpp"

class UnindentAction: public Action {
private:
    int m_row;
    int m_max_indent_width;
    int m_spaces_removed;

    void setSpacesRemoved(const EditorState& state);
    void deleteSpaces(EditorState& state);
    void adjustCursor(EditorState& state);

public:
    UnindentAction(int row, int indent_width);
    UnindentAction(const UnindentAction&) = default;
    ~UnindentAction() = default;

    void apply(ExecutionContext& context) override;

    void undo(EditorState& state) override;

    bool canBeUndone() const override;
};

#endif //UNINDENT_ACTION_HPP
