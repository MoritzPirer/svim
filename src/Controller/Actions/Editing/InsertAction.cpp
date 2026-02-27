#include "../../../../inc/Controller/Actions/Editing/InsertAction.hpp"
#include "../../../../inc/Controller/Actions/ExecutionContext.hpp"

InsertAction::InsertAction(std::vector<std::string> content, Position start, bool supress_merge):
    m_content{content},
    m_start{start},
    m_supress_merge{supress_merge}
    {}

void InsertAction::apply(ExecutionContext& context) {
    context.state.insertLines(m_content, m_start);

    Position first_after_insert;
    if (m_content.empty()) {
        first_after_insert = m_start;
    }
    else {
        first_after_insert = {
            static_cast<int>(m_start.row + m_content.size() - 1),
            static_cast<int>(m_content.back().length() + (m_content.size() == 1? m_start.column : 0))
        };
    }
    context.state.moveCursorTo(first_after_insert);

    context.state.requestBackup();
}

bool InsertAction::canBeUndone() const {
    return true;
}

void InsertAction::undo(EditorState& state) {
    if (m_content.empty()) {
        return;
    }

    Position last_inserted = {
        static_cast<int>(m_start.row + m_content.size() - 1),
        std::max(static_cast<int>(m_content.back().length()) - 1, 0)
    };

    if (m_content.size() == 1) {
        last_inserted.column += m_start.column;
    }
    else if (m_content.back().empty()) {
        last_inserted.row--;
    }

    state.deleteRange(m_start, last_inserted);
    state.moveCursorTo(m_start);

    state.requestBackup();
}


bool InsertAction::canAbsorb(const std::shared_ptr<Action>& action) const {
    auto insert_action = std::dynamic_pointer_cast<InsertAction>(action);

    if (insert_action == nullptr) {
        return false; // not an insert action
    }

    if (this->m_supress_merge || insert_action->m_supress_merge) {
        return false;
    }

    if (insert_action->m_content.empty() || insert_action->m_content.front().empty()) {
        return false; // don't merge empty action
    }

    // TODO: check if they are connected

    // don't merge if not in the same word
    return insert_action->m_content.front().front() != ' ';
}

void InsertAction::absorb(const std::shared_ptr<Action>& action) {
    auto insert_action = std::static_pointer_cast<InsertAction>(action);

    // merge first line of other to last of self
    if (!m_content.empty() && ! insert_action->m_content.empty()) {
        this->m_content.back().append(insert_action->m_content.at(0));
    }

    // append other lines
    for (size_t i = 1; i < insert_action->m_content.size(); i++) {
        this->m_content.push_back(insert_action->m_content.at(i));
    }
}