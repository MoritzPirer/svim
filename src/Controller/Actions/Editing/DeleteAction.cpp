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

bool DeleteAction::canBeUndone() const {
    return true;
}

void DeleteAction::undo(EditorState& state) {
    state.insertLines(m_deleted_content, m_start);
    state.moveCursorTo(m_cursor);
}

bool DeleteAction::canAbsorb(const std::shared_ptr<Action>& action) const {
    auto delete_action = std::dynamic_pointer_cast<DeleteAction>(action);

    if (delete_action == nullptr) {
        return false; // not a delete action
    }
    if (this->m_start.row != delete_action->m_start.row) {
        return false;
    }

    bool is_adjacent = (this->m_start.column - 1 == delete_action->m_end.column) 
        || (this->m_end.column == delete_action->m_start.column - 1);
    
    if (!is_adjacent) {
        return false;
    }

    // TODO: still merging over space
    if (delete_action->m_deleted_content.empty() || delete_action->m_deleted_content.back().empty()) {
        return false; // don't merge empty action
    }

    return (delete_action->m_deleted_content.back().back() != ' ');
}

void DeleteAction::absorb(const std::shared_ptr<Action>& action) {
    auto other = std::static_pointer_cast<DeleteAction>(action);

    // 1. Update the cursor to the latest position

    // 2. Determine merge direction
    // If other ends where this starts, it's a Backspace (Delete Left)
    bool is_backspace = (other->m_end.column + 1 == this->m_start.column);
    if (is_backspace) {
        // PREPEND the text: [Other Text][This Text]
        this->m_start = other->m_start;
        
        if (other->m_deleted_content.empty()) {
            return;
        }

        // Merge the overlapping line (other's last line + our first line)
        std::string overlap = other->m_deleted_content.back() + this->m_deleted_content.front();
        this->m_deleted_content.front() = overlap;

        // If the other action had multiple lines, insert them at the beginning
        for (int i = static_cast<int>(other->m_deleted_content.size()) - 2; i >= 0; --i) {
            this->m_deleted_content.insert(this->m_deleted_content.begin(), other->m_deleted_content.at(i));
        }
    }
    else {
        // APPEND the text: [This Text][Other Text] (Forward Delete)

        this->m_cursor = other->m_cursor;

        // account for text moving over
        this->m_end.column = this->m_start.column + 
            (this->m_deleted_content.back().length() + 
                other->m_deleted_content.front().length()) - 1;

        if (other->m_deleted_content.empty()) {
            return;

        }
        this->m_deleted_content.back().append(other->m_deleted_content.front());

        // Add any subsequent lines from the other action
        for (size_t i = 1; i < other->m_deleted_content.size(); ++i) {
            this->m_deleted_content.push_back(other->m_deleted_content.at(i));
        }
    }
}