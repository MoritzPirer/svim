#include <unordered_map>

#include "../../../inc/Controller/Parsing/SpanResolver.hpp"

namespace {

Position findStopPosition(EditorState& state, RangeSettings settings, Direction direction) {
    Position original_cursor_position = state.getCursor().getPosition();

    auto isDelimiter = [&settings](char c) -> bool {
        return (settings.delimiters.find(c) != std::string::npos);
    };

    auto isAntiDelimiter = [&settings](char c) -> bool {
        return (settings.anti_delimiters.has_value()
            && settings.anti_delimiters->find(c) != std::string::npos);
    };

    bool has_reached_delimiter = false;
    
    std::optional<char> character = state.readCharacterAtCursor();
    bool has_reached_non_delimiter = (!character.has_value() || !isDelimiter(*character));

    int delimiter_balance = 0;
    while (state.canCursorMove(direction)) {

        int row_before = state.getCursor().getRow();
        state.moveCursorSideways(direction);
        int row_after = state.getCursor().getRow();

        character = state.readCharacterAtCursor();

        if (settings.paragraph_is_delimiter && row_before != row_after) {
            if (settings.end_behavior == EndBehavior::STOP_BEFORE_END) {
                state.moveCursorSideways(getOppositeDirection(direction));
                break;
            }

            has_reached_delimiter = true;
        }

        // handle overhang cursor position
        if (!character.has_value()){
            continue;
        }

        if (isDelimiter(*character)) {
            has_reached_delimiter = true;
            
            if (delimiter_balance != 0) {
                delimiter_balance--;
                continue;
            }
            
            if (settings.end_behavior == EndBehavior::STOP_BEFORE_END && has_reached_non_delimiter) {
                //move back off of delimiter
                state.moveCursorSideways(getOppositeDirection(direction));
                break;
            }
            if (settings.end_behavior == EndBehavior::STOP_ON_END) {
                break;
            }
            
            if (settings.anti_delimiters.has_value()) {
                delimiter_balance--;
            }
        }
        else {
            has_reached_non_delimiter = true;

            if (settings.end_behavior == EndBehavior::STOP_AFTER_END && has_reached_delimiter
                && delimiter_balance == 0) {
                break;
            }
            
            if (isAntiDelimiter(*character)) {
                delimiter_balance++;
            }
        }

    }

    // restore cursor position
    Position stop_position = state.getCursor().getPosition();
    state.moveCursorTo(original_cursor_position);

    return stop_position;
}

Position startOfParagraph(EditorState& state, ScopeSettings settings) {
    switch (settings.end_behavior) {
    
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

Position startOfLine(EditorState& state, ScopeSettings settings) {
    int row = state.getCursor().getRow();
    int first_column_of_line = state.getCursor().getColumn() / settings.size.width * settings.size.width;

    switch (settings.end_behavior) {

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

Position startOfScope(EditorState& state, ScopeSettings settings) {
    switch (settings.scope) {
    case Scope::FILE: {
        return {0,0};
    }

    case Scope::PARAGRAPH: {
        return startOfParagraph(state, settings);
    }

    case Scope::LINE: {
        return startOfLine(state, settings);
    }

    case Scope::EXPRESSION: {
        return findStopPosition(
            state,
            {
                .delimiters = settings.delimiters,
                .anti_delimiters = std::nullopt,
                .end_behavior = settings.end_behavior,
                .paragraph_is_delimiter = true
            },
            Direction::LEFT
        );

    }

    case Scope::WORD: {
        return findStopPosition(
            state,
            {
                .delimiters = settings.delimiters,
                .anti_delimiters = std::nullopt,
                .end_behavior = settings.end_behavior,
                .paragraph_is_delimiter = true
            },
            Direction::LEFT
        );
    }


    }
}

Position endOfParagraph(EditorState& state, ScopeSettings settings) {
    int row = state.getCursor().getRow();

    switch (settings.end_behavior) {
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

Position endOfLine(EditorState& state, ScopeSettings settings) {
    int last_of_current_line = (state.getCursor().getColumn() / settings.size.width + 1)
        * settings.size.width - 1;
    int row = state.getCursor().getRow();

    last_of_current_line = std::min(
        static_cast<size_t>(last_of_current_line),
        state.getParagraph(row).length()
    );

    switch (settings.end_behavior) {
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

Position endOfScope(EditorState& state, ScopeSettings settings) {
    switch (settings.scope) {
    case Scope::FILE: {
        int row = state.getNumberOfParagrahps() - 1;
        return {row, static_cast<int>(state.getParagraph(row).length())};
    }

    case Scope::PARAGRAPH: {
        return endOfParagraph(state, settings);
    }

    case Scope::LINE: {
        return endOfLine(state, settings);
    }

    case Scope::EXPRESSION: {
        return findStopPosition(
            state,
            {
                .delimiters = settings.delimiters,
                .anti_delimiters = std::nullopt,
                .end_behavior = settings.end_behavior,
                .paragraph_is_delimiter = true
            },
            Direction::RIGHT
        );

    }

    case Scope::WORD: {
        return findStopPosition(
            state,
            {
                .delimiters = settings.delimiters,
                .anti_delimiters = std::nullopt,
                .end_behavior = settings.end_behavior,
                .paragraph_is_delimiter = true
            },
            Direction::RIGHT
        );
    }

    }
}


}; // anonymous workspace

std::pair<Position, Position> SpanResolver::fromDelimiter(EditorState& state, RangeSettings settings) {
    Position start = findStopPosition(state, settings, Direction::LEFT);

    if (settings.anti_delimiters.has_value()) {
        std::string temp = settings.delimiters;
        settings.delimiters = *settings.anti_delimiters;
        settings.anti_delimiters = temp;
    }

    Position end = findStopPosition(state, settings, Direction::RIGHT);

    return {
        start,
        end
    };
}

std::pair<Position, Position> SpanResolver::fromScope(EditorState& state, ScopeSettings settings) {
    return {
        startOfScope(state, settings),
        endOfScope(state, settings)
    };
}
