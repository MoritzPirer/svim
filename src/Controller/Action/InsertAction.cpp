#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

InsertAction::InsertAction(std::vector<std::string> content, Position start):
    m_content{content},
    m_start{start}
    {}

void InsertAction::apply(ExecutionContext& context) {
    context.state.insertLines(m_content, m_start);

    Position first_after_insert = {
        static_cast<int>(m_start.row + m_content.size() - 1),
        static_cast<int>(m_content.back().length() + (m_content.size() == 1? m_start.column : 0))
    };

    context.state.moveCursorTo(first_after_insert);
}

void InsertAction::undo(EditorState& state) {
    Position last_inserted = {
        static_cast<int>(m_start.row + m_content.size() - 1),
        static_cast<int>(m_content.back().length() + (m_content.size() == 1? m_start.column : 0) - 1)
    };

    state.deleteRange(m_start, last_inserted);
    state.moveCursorTo(m_start);
}

bool InsertAction::canBeUndone() const {
    return true;
}