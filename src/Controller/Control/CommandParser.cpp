#include <unordered_map>

#include "../../../inc/Controller/Control/CommandParser.hpp"
#include "../../../inc/Controller/Control/CommandDetails.hpp"

#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"
#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/ParagraphJoiningAction.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"
#include "../../../inc/Controller/Action/MessageAction.hpp"
#include "../../../inc/Controller/Action/DelimiterMoveAction.hpp"
#include "../../../inc/Controller/Action/ScopeMoveAction.hpp"
#include "../../../inc/Controller/Action/InsertAction.hpp"

using std::make_shared;

ParseResult CommandParser::emptyParse() {
    return {std::nullopt, {}};
}

ParseResult CommandParser::generateCharacterwiseMove(ScreenSize text_area_size) {
    return {ModeType::TOOL_MODE, {
        make_shared<CharwiseMoveAction>(text_area_size, m_details->direction)
    }};
}

ParseResult CommandParser::generateMultiCharacterMove(
    ScreenSize text_area_size, EndBehavior end_behavior) {

    // range or custom delimiter
    if (!m_details->scope.has_value()) {
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
                text_area_size,
                std::string(1, m_details->argument.value()),
                m_details->direction,
                end_behavior,
                false
            )}
        };
    }

    switch (m_details->scope.value()) {
    case Scope::FILE:
    case Scope::PARAGRAPH:
    case Scope::LINE: {
        return {m_details->next_mode, {make_shared<ScopeMoveAction>(
            text_area_size,
            m_details->scope.value(),
            m_details->direction,
            end_behavior
        )}};
    }

    case Scope::EXPRESSION: {
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
                text_area_size,
                m_expression_delimiters,
                m_details->direction,
                end_behavior,
                true
            )}
        };
    }

    case Scope::WORD: {
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
                text_area_size,
                m_word_delimiters,
                m_details->direction,
                end_behavior,
                true
            )}
        };
    }

    default: {
        break;
    }

    }
}

ParseResult CommandParser::generateFileCommand(const Settings& settings) {
    SaveConfirmation confirmation = settings.isEnabled("confirm_save")?
        SaveConfirmation::YES : SaveConfirmation::NO;
        
    std::unordered_map<char, ParseResult> results = {
        {'q', {std::nullopt, {make_shared<QuitAction>(QuitMode::ONLY_IF_SAVED)}}},
        {'x', {std::nullopt, {make_shared<QuitAction>(QuitMode::FORCE_QUIT)}}},
        {'s', {std::nullopt, {make_shared<SaveAction>(confirmation)}}},
        {'Q', {std::nullopt, {
            make_shared<SaveAction>(confirmation),
            make_shared<QuitAction>(QuitMode::ONLY_IF_SAVED)
        }}}

        //TODO: rename, open settings
    };

    if (results.contains(*(m_details->argument))) {
        return results.at(*(m_details->argument));
    }

    //return emptyParse();
    std::string message = "You entered " + std::string(1, *(m_details->argument)) + " which is not a file command i know!";
    return {std::nullopt, {make_shared<MessageAction>(message)}};;
}

ParseResult CommandParser::generatParagraphCreationCommand(ScreenSize text_area_size) {
    ParseResult result = {m_details->next_mode, {
        make_shared<ScopeMoveAction>(
            text_area_size,
            Scope::PARAGRAPH,
            m_details->direction,
            EndBehavior::STOP_BEFORE_END
        ),
        make_shared<ParagraphSplittingAction>()
    }};

    if (m_details->direction == Direction::LEFT) {
        result.actions.emplace_back(make_shared<CharwiseMoveAction>(text_area_size, Direction::UP));
    }

    return result;
}

ParseResult CommandParser::generateHint() {
    switch (m_details->operator_type) {
    case Operator::MOVE_WITHIN_CHUNK:
    case Operator::MOVE_OVER_CHUNK: {
        return {std::nullopt, {
            make_shared<MessageAction>("Complete with a scope or range identifier")
        }};
    }
    case Operator::FILE_ACTION: {
        return {std::nullopt, {
            make_shared<MessageAction>("Complete with an action identifier") // todo refer to help operator
        }};
    }
    
    default: {
        return emptyParse();
    }
    }
}
ParseResult CommandParser::generateActions(ScreenSize text_area_size, const Settings& settings) {
    if (!m_details.has_value() ) {
        return emptyParse();
    }

    if (!m_details->is_complete) {
        return generateHint();
    }

    switch (m_details->operator_type) {
    case Operator::SWITCH_MODE: {
        return {ModeType::TYPING_MODE, {}};
    }

    /// Movement

    case Operator::MOVE_BY_CHARACTER: {
        return generateCharacterwiseMove(text_area_size);
    }

    case Operator::MOVE_FIND: {
        return generateMultiCharacterMove(text_area_size, EndBehavior::STOP_ON_END);
    }
    case Operator::MOVE_WITHIN_CHUNK: {
        return generateMultiCharacterMove(text_area_size, EndBehavior::STOP_BEFORE_END);
    }

    case Operator::MOVE_OVER_CHUNK: {
        return generateMultiCharacterMove(text_area_size, EndBehavior::STOP_AFTER_END);
    }

    /// Editing

    case Operator::ERASE: {
        return {m_details->next_mode, {make_shared<EraseAction>(0)}};
    }

    case Operator::PARAGRAPH_CREATE: {
        return generatParagraphCreationCommand(text_area_size);
    }

    case Operator::PARAGRAPH_JOIN: {
        return {ModeType::TOOL_MODE, {
            make_shared<ScopeMoveAction>(
                text_area_size,
                Scope::PARAGRAPH,
                Direction::RIGHT,
                EndBehavior::STOP_BEFORE_END
            ),
            make_shared<ParagraphJoiningAction>()
        }};
    }

    case Operator::PARAGRAPH_SPLIT: {
        return {ModeType::TOOL_MODE, {make_shared<ParagraphSplittingAction>()}};
    }

    case Operator::REPLACE: {
        return {ModeType::TOOL_MODE, {
            make_shared<EraseAction>(0, false),
            make_shared<InsertAction>(*(m_details->argument)),
            make_shared<CharwiseMoveAction>(text_area_size, Direction::LEFT)
        }};
    }

    ///

    case Operator::FILE_ACTION: {
        return generateFileCommand(settings);
    }


    default: {
        return emptyParse();
    }

    }

}

void CommandParser::parseAsOperator(char input) {
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
        {'i', {
            .operator_type = Operator::SWITCH_MODE,
            .next_mode = ModeType::TYPING_MODE
        }},
        {'e', {
            .operator_type = Operator::ERASE,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'E', {
            .operator_type = Operator::ERASE,
            .next_mode = ModeType::TYPING_MODE
        }},
        {'o', {
            .operator_type = Operator::PARAGRAPH_CREATE,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TYPING_MODE
        }},
        {'O', {
            .operator_type = Operator::PARAGRAPH_CREATE,
            .direction = Direction::LEFT,
            .next_mode = ModeType::TYPING_MODE
        }},
        {'+', {
            .operator_type = Operator::PARAGRAPH_JOIN
        }},
        {'-', {
            .operator_type = Operator::PARAGRAPH_SPLIT
        }}
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
        }},
        {'!', {
            .operator_type = Operator::FILE_ACTION
        }},
        {'f', {
            .operator_type = Operator::MOVE_FIND,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'F', {
            .operator_type = Operator::MOVE_FIND,
            .direction = Direction::LEFT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'r', {
            .operator_type = Operator::REPLACE
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
    
    // Operators that take a scope or range indicator as a parameter
    case Operator::MOVE_WITHIN_CHUNK:
    case Operator::MOVE_OVER_CHUNK: {
        std::optional<Scope> scope = charToScope(input);
        if (scope.has_value()) {
            m_details->scope = scope;
            m_details->is_complete = true;
        }
        else if (isRangeIndicator(input)) {
            m_details->argument = m_details->direction == Direction::RIGHT?
                getClosingRangeIndicator(input) : getOpeningRangeIndicator(input);

            m_details->is_complete = true;
        }
        else {
            m_details = std::nullopt;
            m_details->is_complete = false;
        }
        break;
    }

    // Operators that take an argument
    case Operator::REPLACE:
    case Operator::MOVE_FIND:
    case Operator::FILE_ACTION: {
        m_details->argument = input;
        m_details->is_complete = true;
        break;
    }
    
    default: {
        m_details = std::nullopt;
    }

    }
}

bool CommandParser::isRangeIndicator(char c) {
    return (std::string("\"<[({'").find(c) != std::string::npos);
}

char CommandParser::getOpeningRangeIndicator(char range_indicator) {
    std::unordered_map<char, char> indicators = {
        {'}', '{'},
        {'>', '<'},
        {']', '['},
        {')', '('},
    };

    if (indicators.contains(range_indicator)) {
        return indicators.at(range_indicator);
    }

    // already open or symmetrical
    return range_indicator;
}

char CommandParser::getClosingRangeIndicator(char range_indicator) {
    std::unordered_map<char, char> indicators = {
        {'{', '}'},
        {'<', '>'},
        {'[', ']'},
        {'(', ')'},
    };

    if (indicators.contains(range_indicator)) {
        return indicators.at(range_indicator);
    }

    return range_indicator;
}

std::optional<Scope> CommandParser::charToScope(char c) {
    //c = std::tolower(c); //TODO: Descide if scope is case sensitive or not

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

ParseResult CommandParser::parseInput(char input, ScreenSize text_area_size, const Settings& settings) {
    m_details.has_value()? parseAsParameter(input) : parseAsOperator(input);
    
    return generateActions(text_area_size, settings);
}