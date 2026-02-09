#include <unordered_map>

#include "../../../inc/Controller/Control/CommandParser.hpp"
#include "../../../inc/Controller/Control/CommandDetails.hpp"

#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"
#include "../../../inc/Controller/Action/MessageAction.hpp"
#include "../../../inc/Controller/Action/DelimiterMoveAction.hpp"
#include "../../../inc/Controller/Action/ScopeMoveAction.hpp"

using std::make_shared;

ParseResult CommandParser::emptyParse() {
    return {std::nullopt, {}};
}

ParseResult CommandParser::generateCharacterwiseMove(ScreenSize text_area_size) {
    return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(text_area_size, m_details->direction)}};
}

ParseResult CommandParser::generateMultiCharacterMove(ScreenSize text_area_size, EndBehavior end_behavior) {
    if (!m_details->scope.has_value()) {
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
                text_area_size,
                std::string(1, m_details->argument.value()),
                m_details->direction,
                end_behavior
            )}
        };
    }

    switch (m_details->scope.value()) {
    case Scope::FILE:
    case Scope::PARAGRAPH:
    case Scope::LINE:
        return {m_details->next_mode, {make_shared<ScopeMoveAction>(
            text_area_size,
            m_details->scope.value(),
            m_details->direction,
            end_behavior
        )}};
    case Scope::EXPRESSION:
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
                text_area_size,
                m_expression_delimiters,
                m_details->direction,
                end_behavior
            )}
        };
    case Scope::WORD:
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
                text_area_size,
                m_word_delimiters,
                m_details->direction,
                end_behavior
            )}
        };
    default:
        break;
    }
}

ParseResult CommandParser::generateActions(ScreenSize text_area_size) {
    if (!m_details.has_value() ) {
        return emptyParse();
    }

    if (!m_details->is_complete) {
        return {std::nullopt, {make_shared<MessageAction>("incomplete, but a command was recognized")}};
    }

    switch (m_details->operator_type) {
    case Operator::FILE_ACTION: {
        return {ModeType::TOOL_MODE, {make_shared<QuitAction>(QuitMode::FORCE_QUIT)}};
    }
    case Operator::MOVE_BY_CHARACTER: {
        return generateCharacterwiseMove(text_area_size);
    }
    case Operator::MOVE_WITHIN_CHUNK: 
        return generateMultiCharacterMove(text_area_size, EndBehavior::STOP_BEFORE_END);
    case Operator::MOVE_OVER_CHUNK: {
        return generateMultiCharacterMove(text_area_size, EndBehavior::STOP_AFTER_END);
    }
    default:
        return emptyParse();
    }

}

void CommandParser::parseAsOperator(char input) {
    /*
    switch (input) {
        case 'h':
        case 'j':
        case 'k':
        case 'l':
            m_operator_type = Operator::MOVE_BY_CHARACTER;
            m_argument = input;
            m_is_complete = true;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'i':
            m_operator_type = Operator::SWITCH_MODE;
            m_is_complete = true;
            m_next_mode = ModeType::TYPING_MODE;
            break;
        case 'm':
            m_operator_type = Operator::MOVE_WITHIN_CHUNK;
            m_destination = Destination::END;
            m_is_complete = false;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'M':
            m_operator_type = Operator::MOVE_WITHIN_CHUNK;
            m_destination = Destination::START;
            m_is_complete = false;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'g':
            m_operator_type = Operator::MOVE_OVER_CHUNK;
            m_destination = Destination::END;
            m_is_complete = false;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'G':
            m_operator_type = Operator::MOVE_OVER_CHUNK;
            m_destination = Destination::START;
            m_is_complete = false;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'a':
            m_operator_type = Operator::MOVE_WITHIN_CHUNK;
            m_destination = Destination::END;
            m_is_complete = false;
            m_next_mode = ModeType::TYPING_MODE;
            break;
        case 'A':
            m_operator_type = Operator::MOVE_WITHIN_CHUNK;
            m_destination = Destination::START;
            m_is_complete = false;
            m_next_mode = ModeType::TYPING_MODE;
            break;
        case 'q':
            m_operator_type = Operator::FILE_ACTION;
            m_is_complete = true;
            break;
        case 'f':
            m_operator_type = Operator::FIND;
            m_destination = Destination::END;
            m_is_complete = false;
            m_next_mode = ModeType::TYPING_MODE;
            break;
        
        default: //not a valid operator
            m_operator_type = std::nullopt;
            m_is_complete = false;
    }
    */

    // operators requiring only one input
    std::unordered_map<char, CommandDetails> simple_commands = {
        {'h', {
            .operator_type = Operator::MOVE_BY_CHARACTER,
            .direction = Direction::LEFT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'j', {
            .operator_type = Operator::MOVE_BY_CHARACTER,
            .direction = Direction::DOWN,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'k', {
            .operator_type = Operator::MOVE_BY_CHARACTER,
            .direction = Direction::UP,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'l', {
            .operator_type = Operator::MOVE_BY_CHARACTER,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'q', {
            .operator_type = Operator::FILE_ACTION,
        }},
    };

    //operators requiring a second input
    std::unordered_map<char, CommandDetails> compound_commands = {
        {'m', {
            .operator_type = Operator::MOVE_WITHIN_CHUNK,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'M', {
            .operator_type = Operator::MOVE_WITHIN_CHUNK,
            .direction = Direction::LEFT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'g', {
            .operator_type = Operator::MOVE_OVER_CHUNK,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'G', {
            .operator_type = Operator::MOVE_OVER_CHUNK,
            .direction = Direction::LEFT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'a', {
            .operator_type = Operator::MOVE_WITHIN_CHUNK,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TYPING_MODE
        }},
        {'A', {
            .operator_type = Operator::MOVE_WITHIN_CHUNK,
            .direction = Direction::LEFT,
            .next_mode = ModeType::TYPING_MODE
        }}
    };

    if (simple_commands.contains(input)) {
        m_details = simple_commands.at(input);
        m_details->is_complete = true;
        return;
    }

    if (compound_commands.contains(input)) {
        m_details = compound_commands.at(input);
        m_details->is_complete = false;
        return;
    }
    
    m_details->is_complete = false;
    m_details = std::nullopt;
}

void CommandParser::parseAsParameter(char input) {

    //only need to care about compund commands here
    switch (m_details->operator_type) {
    
    //all ops that take scope or range here
    case Operator::MOVE_WITHIN_CHUNK:
    case Operator::MOVE_OVER_CHUNK: {
        std::optional<Scope> scope = charToScope(input);
        if (scope.has_value()) {
            m_details->scope = scope;
            m_details->is_complete = true;
        }
        else if (isRangeIndicator(input)) {
            m_details->argument = input;
            m_details->is_complete = true;
        }
        else {
            m_details = std::nullopt;
            m_details->is_complete = false;
        }
        break;
    }
    
    default:
        m_details = std::nullopt;
    }
}

bool CommandParser::isRangeIndicator(char c) {
    return (std::string("\"<>[](){}'").find(c) != std::string::npos);
}

std::optional<Scope> CommandParser::charToScope(char c) {
    std::unordered_map<char, Scope> scopes = {
        {'w', Scope::WORD},
        {'e', Scope::EXPRESSION},
        {'l', Scope::LINE},
        {'p', Scope::PARAGRAPH},
        {'f', Scope::FILE},
    };
    
    if (scopes.contains(c)) {
        return scopes.at(c);
    }

    return std::nullopt;
} 

ParseResult CommandParser::tryGenerateHint() {
    return emptyParse();
}

ParseResult CommandParser::parseInput(char input, ScreenSize text_area_size, const Settings& settings) {
    (void) settings;

    if (!m_details.has_value()) {
        parseAsOperator(input);
    }
    else {
        parseAsParameter(input);
    }
    
    return generateActions(text_area_size);
}