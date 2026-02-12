#include <unordered_map>

#include "../../../inc/Controller/Control/CommandParser.hpp"
#include "../../../inc/Controller/Control/CommandDetails.hpp"

#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"
#include "../../../inc/Controller/Action/DelimiterMoveAction.hpp"
#include "../../../inc/Controller/Action/ScopeMoveAction.hpp"

#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"

#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Action/EraseAction.hpp"

#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/ParagraphJoiningAction.hpp"

#include "../../../inc/Controller/Action/IndentAction.hpp"
#include "../../../inc/Controller/Action/UnindentAction.hpp"

#include "../../../inc/Controller/Action/DelimiterCaseSetAction.hpp"
#include "../../../inc/Controller/Action/ScopeCaseSetAction.hpp"

#include "../../../inc/Controller/Action/MessageAction.hpp"

using std::make_shared;

ParseResult CommandParser::parseInput(char input, ScreenSize text_area_size, const Settings& settings) {
    m_details.has_value()? parseAsParameter(input) : parseAsOperator(input);
    
    ParseResult result = generateActions(text_area_size, settings);
    if (m_details.has_value() && m_details->is_complete) {
        m_details = std::nullopt;
    }

    return result;
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
        }},
        {'>', {
            .operator_type = Operator::INDENT,
        }},
        {'<', {
            .operator_type = Operator::UNINDENT,
        }}
    };

    //operators requiring a second input
    std::unordered_map<char, CommandDetails> compound_commands = {
        {'m', {
            .operator_type = Operator::MOVE_WITHIN_CHUNK,
            .scope = std::nullopt,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'M', {
            .operator_type = Operator::MOVE_WITHIN_CHUNK,
            .scope = std::nullopt,
            .direction = Direction::LEFT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'g', {
            .operator_type = Operator::MOVE_OVER_CHUNK,
            .scope = std::nullopt,
            .direction = Direction::RIGHT,
            .next_mode = ModeType::TOOL_MODE
        }},
        {'G', {
            .operator_type = Operator::MOVE_OVER_CHUNK,
            .scope = std::nullopt,
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
        }},
        {'t', {
            .operator_type = Operator::CASE_SET_LOWER,
            .scope = std::nullopt
        }},
        {'T', {
            .operator_type = Operator::CASE_SET_UPPER,
            .scope = std::nullopt
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
    case Operator::CASE_SET_LOWER:
    case Operator::CASE_SET_UPPER:
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
                EndBehavior::STOP_BEFORE_END,
                Scope::PARAGRAPH,
                Direction::RIGHT
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

    case Operator::INDENT: {
        return {ModeType::TOOL_MODE, {
            make_shared<IndentAction>(settings.isEnabled("do_skinny_tabs")? 2 : 4)
        }};
    }

    case Operator::UNINDENT: {
        return {ModeType::TOOL_MODE, {
            make_shared<UnindentAction>(settings.isEnabled("do_skinny_tabs")? 2 : 4)
        }};
    }

    case Operator::CASE_SET_LOWER: {
        return generateCaseSetCommand(text_area_size, Case::LOWER_CASE);
    }

    case Operator::CASE_SET_UPPER: {
        return generateCaseSetCommand(text_area_size, Case::UPPER_CASE);
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

ParseResult CommandParser::emptyParse() {
    return {std::nullopt, {}};
}

ParseResult CommandParser::generateHint() {
    // only applies to compound commands
    switch (m_details->operator_type) {

    case Operator::MOVE_WITHIN_CHUNK:
    case Operator::MOVE_OVER_CHUNK:
    case Operator::CASE_SET_UPPER:
    case Operator::CASE_SET_LOWER: {
        return {std::nullopt, {
            make_shared<MessageAction>("Enter a scope (w,e,p,l,f) or a range identifier ( \",',(,[, {, <)!")
        }};
    }
    
    case Operator::MOVE_FIND:
    case Operator::REPLACE: {
        return {std::nullopt, {
            make_shared<MessageAction>("Enter a character argument (letter, digit, special character)!")
        }};

    }

    case Operator::FILE_ACTION: {
        return {std::nullopt, {
            make_shared<MessageAction>("Enter an action identifier! Refer to the help command for more info!")
        }};
    }
    
    default: {
        return emptyParse();
    }
    }
}

ParseResult CommandParser::generateCharacterwiseMove(ScreenSize text_area_size) {
    return {ModeType::TOOL_MODE, {
        make_shared<CharwiseMoveAction>(text_area_size, m_details->direction)
    }};
}

ParseResult CommandParser::generateMultiCharacterMove(ScreenSize text_area_size, EndBehavior end_behavior) {

    // range or custom delimiter
    if (!m_details->scope.has_value()) {
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
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
            end_behavior,
            m_details->scope.value(),
            m_details->direction
        )}};
    }

    case Scope::EXPRESSION: {
        return {m_details->next_mode, {
            make_shared<DelimiterMoveAction>(
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

ParseResult CommandParser::generateCaseSetCommand(ScreenSize text_area_size, Case target_case) {
    // range given
    if (!m_details->scope.has_value()) {
        return {std::nullopt, {
            make_shared<DelimiterCaseSetAction>(
                std::string(1, getOpeningRangeIndicator(*(m_details->argument))),
                Direction::LEFT,
                false,
                target_case
            ),
            make_shared<DelimiterCaseSetAction>(
                std::string(1, getClosingRangeIndicator(*(m_details->argument))),
                Direction::RIGHT,
                false,
                target_case
            )
        }};
    }

    switch (*(m_details->scope)) {
    case Scope::FILE: 
    case Scope::PARAGRAPH:
    case Scope::LINE: {
        return {std::nullopt, {
            make_shared<ScopeCaseSetAction>(
                text_area_size,
                *(m_details->scope),
                target_case
            )
        }};
    }
    case Scope::EXPRESSION:
    case Scope::WORD: {
        std::string delimiters =
            (*(m_details->scope) == Scope::EXPRESSION? m_expression_delimiters : m_word_delimiters);

        return {std::nullopt, {
            make_shared<DelimiterCaseSetAction>(
                delimiters,
                Direction::LEFT,
                true,
                target_case
            ),
            make_shared<DelimiterCaseSetAction>(
                delimiters,
                Direction::RIGHT,
                true,
                target_case
            )
        }};
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
            EndBehavior::STOP_BEFORE_END,
            Scope::PARAGRAPH,
            m_details->direction
        ),
        //make_shared<CharwiseMoveAction>(text_area_size, Direction::RIGHT),
        make_shared<ParagraphSplittingAction>()
    }};

    if (m_details->direction == Direction::LEFT) {
        result.actions.emplace_back(make_shared<CharwiseMoveAction>(text_area_size, Direction::UP));
    }

    return result;
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

    // already closed or symmetrical
    return range_indicator;
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