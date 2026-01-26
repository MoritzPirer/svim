#include "../../../inc/Controller/Action/DeleteAction.hpp"

void DeleteAction::applyTo(EditorState& state) {
    Position start = (m_start.has_value()? m_start.value() : state.getCursor().getPosition());
    Position end = (m_end.has_value()? m_end.value() : state.getCursor().getPosition());

    if (end.column == 0) {
        state.joinLineToPrevious(end.row);
        return;
    }
    // erase **before** cursor
    start.column = std::max(0, start.column - 1);
    end.column = std::max(0, end.column - 1);

    state.deleteRange(start, end);
    state.moveCursorLeft();

    state.getFile().setHasUnsavedChanges(true);
}