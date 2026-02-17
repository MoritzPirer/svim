#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

DeleteAction::DeleteAction(Position start, Position end):
    m_deleted_content{},
    m_start{start},
    m_end{end}
    {}

void DeleteAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    m_deleted_content = state.copyRange(m_start, m_end);
    state.deleteRange(m_start, m_end);
    state.moveCursorTo(m_start);
}

void DeleteAction::undo(EditorState& state) {
    state.insertLines(m_deleted_content, m_start);
    state.moveCursorTo({m_end.row, m_end.column + 1});
}

bool DeleteAction::canBeUndone() const {
    return true;
}