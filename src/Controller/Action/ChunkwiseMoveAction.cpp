#include "../../../inc/Controller/Action/ChunkwiseMoveAction.hpp"

ChunkwiseMoveAction::ChunkwiseMoveAction(Scope scope, Destination destination):
    m_scope{scope},
    m_destination{destination}
    {}

void ChunkwiseMoveAction::fileScopeMove(EditorState& state) {
    switch (m_destination) {
        case Destination::START: {
            state.moveCursorTo({0, 0});
            break;
        }
        case Destination::END: {
            state.moveCursorTo({
                static_cast<int>(state.getNumberOfParagrahps() - 1),
                static_cast<int>(state.getParagraph(state.getNumberOfParagrahps() - 1).length())
            });
        }
    }
}

void ChunkwiseMoveAction::paragraphScopeMove(EditorState& state) {
    switch (m_destination) {
        case Destination::START: {
            state.moveCursorTo({state.getCursor().getRow(), 0});
            break;
        }
        case Destination::END: {
            state.moveCursorTo({
                state.getCursor().getRow(),
                static_cast<int>(state.getParagraph(state.getCursor().getRow()).length())
            });
        }
    }
}

void ChunkwiseMoveAction::applyTo(EditorState& state) {
    switch (m_scope) {
        case Scope::FILE: {
            fileScopeMove(state);
            break;
        }
        case Scope::PARAGRAPH: {
            paragraphScopeMove(state);
            break;
        }
        default:
            throw std::invalid_argument(
                "The provided scope is invalid! Did you mean to use DirectionalMoveAction?");
    }
}