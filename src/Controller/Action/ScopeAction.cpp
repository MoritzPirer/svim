#include "../../../inc/Controller/Action/ScopeAction.hpp"

ScopeAction::ScopeAction(
    ScreenSize size,
    EndBehavior end_behavior
):
    m_size{size},
    m_end_behavior{end_behavior}
    {}

Position ScopeAction::startOfParagraph(const EditorState& state) {
    switch (m_end_behavior) {
    
    // go to the last character of the previous paragraph (if it exists)
    case EndBehavior::STOP_AFTER_END:
    case EndBehavior::STOP_ON_END: {
        int row = state.getCursor().getRow();
        if (row == 0) {
            return {row, 0};
        }

        row--;
        return {row, static_cast<int>(state.getParagraph(row).length() - 1)};
    }

    case EndBehavior::STOP_BEFORE_END: {
        return {state.getCursor().getRow(), 0};
    }
    }
}

Position ScopeAction::startOfLine(const EditorState& state) {
    int row = state.getCursor().getRow();
    int first_column_of_line = state.getCursor().getColumn() / m_size.width * m_size.width;

    switch (m_end_behavior) {

    // go to the last character of the previous line (if it exists)
    case EndBehavior::STOP_AFTER_END:
    case EndBehavior::STOP_ON_END: {
        if (first_column_of_line > 0) { 
            return {row, first_column_of_line - 1};
        }
        if (row > 0) {
            return {row - 1, static_cast<int>(state.getParagraph(row - 1).length() - 1)};
        }

        return {row, first_column_of_line};
    }

    //go to the first character of the current line
    case EndBehavior::STOP_BEFORE_END: {
        return {state.getCursor().getRow(), first_column_of_line};
    }

    }
}

Position ScopeAction::startOfScope(const EditorState& state, Scope scope) {
    switch (scope) {
    case Scope::FILE: {
        return {0,0};
    }

    case Scope::PARAGRAPH: {
        return startOfParagraph(state);
    }

    case Scope::LINE: {
        return startOfLine(state);
    }

    default: {
        throw std::invalid_argument("ScopeAction is not indended for Word and Expression scopes!");
    }

    }
}

Position ScopeAction::endOfParagraph(const EditorState& state) {
    int row = state.getCursor().getRow();

    switch (m_end_behavior) {
    case EndBehavior::STOP_AFTER_END:
    case EndBehavior::STOP_ON_END: {
        if (static_cast<size_t>(row) >= state.getNumberOfParagrahps() - 1) {
            return {row, static_cast<int>(state.getParagraph(row).length() - 1)};
        }
        
        return {row + 1, 0};
    }

    case EndBehavior::STOP_BEFORE_END: {
        int column = state.getParagraph(row).length();
        return {row, column};
    }

    }
}

Position ScopeAction::endOfLine(const EditorState& state) {
    int last_of_current_line = (state.getCursor().getColumn() / m_size.width + 1) * m_size.width - 1;
    int row = state.getCursor().getRow();

    last_of_current_line = std::min(
        static_cast<size_t>(last_of_current_line),
        state.getParagraph(row).length()
    );

    switch (m_end_behavior) {
    case EndBehavior::STOP_AFTER_END:
    case EndBehavior::STOP_ON_END: {
        
        if (static_cast<size_t>(last_of_current_line) < state.getParagraph(row).length()) {
            return {row, last_of_current_line + 1};
        }

        if (static_cast<size_t>(row) == state.getNumberOfParagrahps() - 1) {
            return {row, last_of_current_line};
        }

        return {row + 1, 0};
    }

    case EndBehavior::STOP_BEFORE_END: {

        return {row, last_of_current_line};
    }
    }

}

Position ScopeAction::endOfScope(const EditorState& state, Scope scope) {
    switch (scope) {
    case Scope::FILE: {
        int row = state.getNumberOfParagrahps() - 1;
        return {row, static_cast<int>(state.getParagraph(row).length() - 1)};
    }

    case Scope::PARAGRAPH: {
        return endOfParagraph(state);
    }

    case Scope::LINE: {
        return endOfLine(state);
    }

    default: {
        throw std::invalid_argument("ScopeAction is not indended for Word and Expression scopes!");
    }
    }
}