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

    state.requestBackup();
}

bool DeleteAction::canBeUndone() const {
    return true;
}

void DeleteAction::undo(EditorState& state) {
    if (m_deleted_content.empty()) {
        state.splitAt(m_start);
    }
    else {
        state.insertLines(m_deleted_content, m_start);
    }
    state.moveCursorTo(m_cursor);

    state.requestBackup();
}

bool DeleteAction::isAdjacent(std::shared_ptr<DeleteAction> other) const {
    if (this->m_start.row != other->m_start.row) {
        return false;
    }

    return (this->m_start.column - 1 == other->m_end.column) 
        || (other->m_start.column - 1 == this->m_end.column);
}

bool DeleteAction::hasContent() const {
    for (const std::string& paragraph : m_deleted_content) {
        if (!paragraph.empty()) {
            return true;
        }
    }

    return false;
}

bool DeleteAction::canAbsorb(const std::shared_ptr<Action>& action) const {
    auto delete_action = std::dynamic_pointer_cast<DeleteAction>(action);

    // not a delete action
    if (delete_action == nullptr) {
        return false; 
    }

    if (!isAdjacent(delete_action)) {
        return false;
    }

    if (!delete_action->hasContent()) {
        return false; 
    }

    return !std::isspace(delete_action->m_deleted_content.back().back());
}

void DeleteAction::absorbBeforeCurrent(std::shared_ptr<DeleteAction> other) {
    this->m_start = other->m_start;
    
    if (other->m_deleted_content.empty()) {
        return;
    }

    std::string overlap = other->m_deleted_content.back() + this->m_deleted_content.front();
    this->m_deleted_content.front() = overlap;

    for (int i = static_cast<int>(other->m_deleted_content.size()) - 2; i >= 0; i--) {
        this->m_deleted_content.insert(this->m_deleted_content.begin(), other->m_deleted_content.at(i));
    }
}

void DeleteAction::absorbAfterCurrent(std::shared_ptr<DeleteAction> other) {
    this->m_cursor = other->m_cursor;

    if (other->m_deleted_content.empty()) {
        return;
    }

    // account for text moving over
    this->m_end.column = this->m_start.column + 
        (this->m_deleted_content.back().length() + other->m_deleted_content.front().length()) - 1;

    this->m_deleted_content.back().append(other->m_deleted_content.front());

    // Add any subsequent lines from the other action
    for (size_t i = 1; i < other->m_deleted_content.size(); ++i) {
        this->m_deleted_content.push_back(other->m_deleted_content.at(i));
    }
}

void DeleteAction::absorb(const std::shared_ptr<Action>& action) {
    auto other = std::static_pointer_cast<DeleteAction>(action);

    bool is_backspace = (other->m_end.column + 1 == this->m_start.column);

    if (is_backspace) {
        absorbBeforeCurrent(other);
    }
    else {
        absorbAfterCurrent(other);
    }
}