#include "../../../../inc/Controller/Actions/Editing/DeleteAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

DeleteAction::DeleteAction(Position start, Position end, Position cursor):
    m_deleted_content{},
    m_start{start},
    m_end{end},
    m_cursor{cursor}
    {}

void DeleteAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    m_deleted_content = state.copyRange(m_start, m_end);
    state.deleteRange(m_start, m_end);
    state.moveCursorTo(m_start);
}

void DeleteAction::undo(EditorState& state) {
    state.insertLines(m_deleted_content, m_start);
    state.moveCursorTo(m_cursor);
}

bool DeleteAction::canBeUndone() const {
    return true;
}