#include "../../../inc/Controller/Action/DirectionalMoveAction.hpp"

DirectionalMoveAction::DirectionalMoveAction(Scope scope, Destination destination,
    ScreenSize size, Direction direction):

    ChunkwiseMoveAction(scope, destination),
    SizeDependantAction(size),
    m_direction{direction}
    {
        if (direction == Direction::UP || direction == Direction::DOWN) {
            throw std::invalid_argument("DirectionalMoveAction currently only supports horizontal moves!");
        }
    }


/*
scope is handled
destination, direction
w/W moves to the next word start: forward, start
e/E moves to the next word end: forward, end
b/B moves to the previous word start: backwards, end

must guarantee cursor movement except when impossible
*/

std::string DirectionalMoveAction::getDelimiters() const {
    switch (m_scope) {
        case Scope::WORD: {
            return " \t!\"§$%&/()=?`*+'#-_.:,;\\";
        }
        case Scope::PHRASE: {
            return " \t";
        }
        default:
            throw std::invalid_argument("Invalid scope! Did you mean to use CharwiseMoveAction?");
    }
}

void DirectionalMoveAction::moveUntilDelimiter(EditorState& state) const {
    const std::string delimiters = getDelimiters();

    bool has_moved = false;
    while (state.canCursorMove(m_direction)) {
        state.moveCursor(m_direction, m_size.width);
        std::optional<char> character = state.readCharacterAtCursor();

        // handle overhang cursor position
        if (!character.has_value()){
            if (state.canCursorMove(m_direction)) {
                continue;
            }

            break;
        }

        // stop at first delimiter after moving at least once
        if (delimiters.find(*character) != std::string::npos) {
            if (has_moved == true) {
                state.moveCursor(getOppositeDirection(m_direction), m_size.width);
                break;
            }
        }
        else {
            has_moved = true;
        }
    } 
}

void DirectionalMoveAction::moveOverDelimiter(EditorState& state) const {
    const std::string delimiters = getDelimiters();

    bool has_reached_delimiter = false;
    while (state.canCursorMove(m_direction)) {
        state.moveCursor(m_direction, m_size.width);
        std::optional<char> character = state.readCharacterAtCursor();

        // handle overhang cursor position
        if (!character.has_value()) {
            if (state.canCursorMove(m_direction)) {
                continue;
            }

            break;
        }

        // stop at first non-delimiter character after encountering at least one
        if (delimiters.find(*character) != std::string::npos) {
            has_reached_delimiter = true;
        }
        else if (has_reached_delimiter == true) {
            break;
        }
    }
}

void DirectionalMoveAction::applyTo(EditorState& state) {
    if (m_direction == Direction::FORWARD) {
        switch (m_destination) {
            case Destination::END: {
                moveUntilDelimiter(state);
                break;
            }
            case Destination::START: {
                moveOverDelimiter(state);
                break;
            }
        }
    }
    else if (m_direction == Direction::BACKWARD) {
        switch (m_destination) {
            case Destination::END: {
                moveOverDelimiter(state);
                break;
            }
            case Destination::START: {
                moveUntilDelimiter(state);
                break;
            }
        }

    }
}