#include "../../../inc/Controller/Action/MoveWithinChunkAction.hpp"

MoveWithinChunkAction::MoveWithinChunkAction(Scope scope, Destination destination, ScreenSize size):
    m_scope{scope},
    m_delimiters{std::nullopt},
    m_destination{destination},
    m_size{size}
    {}

MoveWithinChunkAction::MoveWithinChunkAction(std::string delimiters, Destination destination, ScreenSize size):
    m_scope{std::nullopt},
    m_delimiters{delimiters},
    m_destination{destination},
    m_size{size}
    {}

void MoveWithinChunkAction::applyTo(EditorState& state) {
    if (m_scope.has_value()) {
        switch (*m_scope) {
            case Scope::FILE:
                return fileScopeMove(state);
            case Scope::PARAGRAPH:
                return paragraphScopeMove(state);
            case Scope::LINE:
                return lineScopeMove(state);
            default:
                throw std::logic_error("should've been created with delimiter constructor!");
        }
    }

    return delimiterMove(state);
}

void MoveWithinChunkAction::fileScopeMove(EditorState& state) {
    switch (m_destination) {
        case Destination::START: {
            state.moveCursorTo({0, 0});
            return;
        }
        case Destination::END: {
            int row = state.getNumberOfParagrahps() - 1;
            int column = state.getParagraph(row).length();
            state.moveCursorTo({row, column});
            return;
        }
    }
}

void MoveWithinChunkAction::paragraphScopeMove(EditorState& state) {
    switch (m_destination) {
        case Destination::START: {
            state.moveCursorTo({state.getCursor().getRow(), 0});
            return;
        }
        case Destination::END: {
            int row = state.getCursor().getRow();
            int column = state.getParagraph(row).length();
            state.moveCursorTo({row, column});
            return;
        }
    }

}

void MoveWithinChunkAction::lineScopeMove(EditorState& state) {
    switch (m_destination) {
        case Destination::START: {
            int new_column = state.getCursor().getColumn() / m_size.width * m_size.width;
            state.moveCursorTo({state.getCursor().getRow(), new_column});
            return;
        }
        case Destination::END: {
            int new_column = state.getCursor().getColumn() / m_size.width * m_size.width + m_size.width - 1;
            new_column = std::min(static_cast<size_t>(new_column),
                state.getParagraph(state.getCursor().getRow()).length());

            state.moveCursorTo({state.getCursor().getRow(), new_column});
            return;
        }
    }
    
}

void MoveWithinChunkAction::delimiterMove(EditorState& state) {

    //TODO HANDLE END OF EXPRESSION == END OF PARAGRAPH
    Direction direction = (m_destination == Destination::END? Direction::FORWARD : Direction::BACKWARD);
    while (state.canCursorMove(direction)) {
        state.moveCursor(direction, m_size.width);

        std::optional<char> character = state.readCharacterAtCursor();

        // handle overhang cursor position
        if (!character.has_value()){
            bool cross_into_next_paragraph = false;
            if (cross_into_next_paragraph) {
                continue;
            }

            state.moveCursor(getOppositeDirection(direction), m_size.width);
            break;
        }

        // stop at first delimiter after moving at least once
        if (m_delimiters->find(*character) != std::string::npos) {
            state.moveCursor(getOppositeDirection(direction), m_size.width);
            break;
        }
    }
}