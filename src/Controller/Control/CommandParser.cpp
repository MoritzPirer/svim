#include <unordered_map>

#include "../../../inc/Controller/Control/CommandParser.hpp"

#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"
#include "../../../inc/Controller/Action/ChunkwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/DirectionalMoveAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/MoveToAction.hpp"
#include "../../../inc/Controller/Action/MoveWithinChunkAction.hpp"
#include "../../../inc/Controller/Action/MessageAction.hpp"

using std::make_shared;

ParseResult CommandParser::emptyParse() {
    return {std::nullopt, {}};
}

ParseResult CommandParser::generateCharacterwiseMove(ScreenSize text_area_size) {
    Direction direction;
    switch (*m_argument) {
        case 'h': {
            direction = Direction::BACKWARD;
            break;
        }
        case 'j': {
            direction = Direction::DOWN;
            break;
        }
        case 'k': {
            direction = Direction::UP;
            break;
        }
        case 'l': {
            direction = Direction::FORWARD;
            break;
        }
    }
    
    return {ModeType::TOOL_MODE, {make_shared<CharwiseMoveAction>(text_area_size, direction)}};
}

ParseResult CommandParser::generateMoveWithinChunk(ScreenSize text_area_size, ModeType mode) {
    if (m_scope.has_value()) {
        switch (*m_scope) {
            case Scope::FILE:
            case Scope::PARAGRAPH:
            case Scope::LINE:
                return {mode, {make_shared<MoveWithinChunkAction>(*m_scope, *m_destination, text_area_size)}};
            case Scope::EXPRESSION:
                return {mode, {
                    make_shared<MoveWithinChunkAction>(m_expression_delimiters, *m_destination, text_area_size)}
                };
            case Scope::WORD:
                return {mode, {
                    make_shared<MoveWithinChunkAction>(m_word_delimiters, *m_destination, text_area_size)}
                };

        }
    }

    return emptyParse();
}

ParseResult CommandParser::generateActions(ScreenSize text_area_size) {
    switch (*m_operator_type) {
        case OperatorType::FILE_ACTION: {
            return {ModeType::TOOL_MODE, {make_shared<QuitAction>(true)}};
        }
        case OperatorType::MOVE_BY_CHARACTER: {
            return generateCharacterwiseMove(text_area_size);
        }
        case OperatorType::MOVE_WITHIN_CHUNK: {
            return generateMoveWithinChunk(text_area_size, *m_next_mode);
        }

        default: break;
    }

    return emptyParse();
}

void CommandParser::parseAsOperator(char input) {
    switch (input) {
        case 'h':
        case 'j':
        case 'k':
        case 'l':
            m_operator_type = OperatorType::MOVE_BY_CHARACTER;
            m_argument = input;
            m_is_complete = true;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'i':
            m_operator_type = OperatorType::SWITCH_MODE;
            m_is_complete = true;
            m_next_mode = ModeType::TYPING_MODE;
            break;
        case 'm':
            m_operator_type = OperatorType::MOVE_WITHIN_CHUNK;
            m_destination = Destination::END;
            m_is_complete = false;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'M':
            m_operator_type = OperatorType::MOVE_WITHIN_CHUNK;
            m_destination = Destination::START;
            m_is_complete = false;
            m_next_mode = ModeType::TOOL_MODE;
            break;
        case 'a':
            m_operator_type = OperatorType::MOVE_WITHIN_CHUNK;
            m_destination = Destination::END;
            m_is_complete = false;
            m_next_mode = ModeType::TYPING_MODE;
            break;
        case 'A':
            m_operator_type = OperatorType::MOVE_WITHIN_CHUNK;
            m_destination = Destination::START;
            m_is_complete = false;
            m_next_mode = ModeType::TYPING_MODE;
            break;
        case 'q':
            m_operator_type = OperatorType::FILE_ACTION;
            m_is_complete = true;
            break;
        
        default: //not a valid operator
            m_operator_type = std::nullopt;
            m_is_complete = false;
    }
}

bool CommandParser::operatorExpectsScopeOrRange() {
    std::unordered_map<OperatorType, bool> is_expected = {
        {OperatorType::FILE_ACTION, false},
        {OperatorType::MOVE_BY_CHARACTER, false},
        {OperatorType::MOVE_BY_CHUNK, true},
        {OperatorType::MOVE_WITHIN_CHUNK, true},
        {OperatorType::FIND, false}
    };

    if (!m_operator_type.has_value()) {
        return false;
    }

    return is_expected.at(*m_operator_type);
}

void CommandParser::parseAsScopeOrRange(char input) {
    std::unordered_map<char, Scope> scope = {
        {'w', Scope::WORD},
        {'e', Scope::EXPRESSION},
        {'l', Scope::LINE},
        {'p', Scope::PARAGRAPH},
        {'f', Scope::FILE}
    };
    
    if (scope.contains(input)) {
        m_scope = scope.at(input);
        m_is_complete = true;
    }
    
}

ParseResult CommandParser::tryGenerateHint() {
    return emptyParse();
}
 
ParseResult CommandParser::parseInput(char input, ScreenSize text_area_size, const Settings& settings) {
    (void) settings;

    if (m_operator_type.has_value()) {
        //parse as second keystroke
        if (operatorExpectsScopeOrRange()) {
            parseAsScopeOrRange(input);
        }
    }
    else {
        parseAsOperator(input);
    }
    
    if (m_is_complete) {
        ParseResult result = generateActions(text_area_size);
        m_operator_type = std::nullopt;
        return result;
    }
    else {
        //return tryGenerateHint();
        if (m_operator_type.has_value()) {
            return {std::nullopt, {make_shared<MessageAction>("not complete, but you typed " + std::string(1, input))}};
        }
        else return emptyParse();
    }
}