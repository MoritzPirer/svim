#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include <ncurses.h>
void DeleteAction::applyTo(EditorState& state, ScreenSize size) {
    (void) size;
    Position start = (m_start.has_value()? m_start.value() : state.getCursor().getPosition());
    Position end = (m_end.has_value()? m_end.value() : state.getCursor().getPosition());

    // erase **before** cursor
    start.column = std::max(0, start.column - 1);
    end.column = std::max(0, end.column - 1);

    state.deleteRange(start, end);
    state.moveCursorLeft();
}