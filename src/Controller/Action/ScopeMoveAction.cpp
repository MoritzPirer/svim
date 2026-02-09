#include "../../../inc/Controller/Action/ScopeMoveAction.hpp"

ScopeMoveAction::ScopeMoveAction(
    ScreenSize size,
    Scope scope,
    Direction move_direction,
    EndBehavior end_behavior
):
    m_size{size},
    m_scope{scope},
    m_move_direction{move_direction},
    m_end_behavior{end_behavior}
    {}

void ScopeMoveAction::fileScopeMove(EditorState& state) {
    switch (m_move_direction) {
    case Direction::LEFT: {
        state.moveCursorTo({0, 0});
        return;
    }
    case Direction::RIGHT: {
        int row = state.getNumberOfParagrahps() - 1;
        int column = state.getParagraph(row).length();
        state.moveCursorTo({row, column});
        return;
    }
    default:
        return;
    }
}

void ScopeMoveAction::paragraphScopeMove(EditorState& state) {
    switch (m_move_direction) {
    case Direction::LEFT: {
        if (m_end_behavior == EndBehavior::STOP_BEFORE_END) {
            state.moveCursorTo({state.getCursor().getRow(), 0});
        }
        else {
            int row = state.getCursor().getRow();
            if (row > 0) {
                row--;
                state.moveCursorTo({ row, static_cast<int>(state.getParagraph(row).length())});
            }
        }
        return;
    }

    case Direction::RIGHT: {
        if (m_end_behavior == EndBehavior::STOP_BEFORE_END) {
            int row = state.getCursor().getRow();
            int column = state.getParagraph(row).length();
            state.moveCursorTo({row, column});
        }
        else {
            int row = state.getCursor().getRow();
            if (static_cast<size_t>(row) < state.getNumberOfParagrahps() - 1) {
                row++;
                state.moveCursorTo({row, 0});
            }
        }
        return;
    }
    default:
        return;
    }
}

void ScopeMoveAction::lineScopeMove(EditorState& state) {
    switch (m_move_direction) {
    case Direction::LEFT: {
        int new_column = state.getCursor().getColumn() / m_size.width * m_size.width;
        state.moveCursorTo({state.getCursor().getRow(), new_column});

        if (m_end_behavior == EndBehavior::STOP_AFTER_END) {
            state.moveCursorLeft();
        }

        return;
    }
    case Direction::RIGHT: {
        int new_column = state.getCursor().getColumn() / m_size.width * m_size.width + m_size.width - 1;
        new_column = std::min(
            static_cast<size_t>(new_column),
            state.getParagraph(state.getCursor().getRow()).length()
        );

        state.moveCursorTo({state.getCursor().getRow(), new_column});

        if (m_end_behavior == EndBehavior::STOP_AFTER_END) {
            state.moveCursorRight();
        }

        return;
    }
    default:
        return;
    }
}

void ScopeMoveAction::applyTo(EditorState& state) {
    switch (m_scope) {
        case Scope::FILE:
            return fileScopeMove(state);
        case Scope::PARAGRAPH:
            return paragraphScopeMove(state);
        case Scope::LINE:
            return lineScopeMove(state);
        default:
            throw std::logic_error("Use DelimiterMoveAction for Expression and Word scope!");
    }
}
