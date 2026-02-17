#include <unordered_map>


#include "../../../inc/Controller/Control/CommandParser.hpp"
#include "../../../inc/Controller/Control/CommandDetails.hpp"

#include "../../../inc/Controller/Action/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"

#include "../../../inc/Controller/Control/SectionResolver.hpp"
#include "../../../inc/Controller/Action/SectionMoveAction.hpp"

#include "../../../inc/Controller/Action/CompoundAction.hpp"

#include "../../../inc/Controller/Action/SaveAction.hpp"
#include "../../../inc/Controller/Action/QuitAction.hpp"

#include "../../../inc/Controller/Action/InsertAction.hpp"
#include "../../../inc/Controller/Action/DeleteAction.hpp"

#include "../../../inc/Controller/Action/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Action/ParagraphJoiningAction.hpp"

#include "../../../inc/Controller/Action/IndentAction.hpp"
#include "../../../inc/Controller/Action/UnindentAction.hpp"

#include "../../../inc/Controller/Action/CaseSetAction.hpp"

#include "../../../inc/Controller/Action/MessageAction.hpp"
#include "../../../inc/Controller/Action/UndoAction.hpp"
#include "../../../inc/Controller/Control/ParsingContext.hpp"

using std::make_shared;

ParseResult CommandParser::parseInput(char input, ParsingContext context) {
    m_details.has_value()? parseAsParameter(input) : parseAsOperator(input);
    
    ParseResult result = generateActions(context);
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
        }},
        {'u', {
            .operator_type = Operator::UNDO,
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
            m_details->argument = getOpeningRangeIndicator(input);
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

ParseResult CommandParser::generateActions(ParsingContext context) {
    if (!m_details.has_value() ) {
        return emptyParse();
    }

    if (!m_details->is_complete) {
        return generateHint();
    }

    switch (m_details->operator_type) {
    case Operator::SWITCH_MODE: {
        return {ModeType::TYPING_MODE, std::nullopt};
    }

    /// Movement

    case Operator::MOVE_BY_CHARACTER: {
        return generateCharacterwiseMove(context.text_area_size);
    }

    case Operator::MOVE_FIND: {
        return generateMultiCharacterMove(context, EndBehavior::STOP_ON_END);
    }

    case Operator::MOVE_WITHIN_CHUNK: {
        return generateMultiCharacterMove(context, EndBehavior::STOP_BEFORE_END);
    }

    case Operator::MOVE_OVER_CHUNK: {
        return generateMultiCharacterMove(context, EndBehavior::STOP_AFTER_END);
    }

    /// Editing

    case Operator::ERASE: {
        Position cursor = context.state.getCursor().getPosition();
        return {m_details->next_mode, make_shared<DeleteAction>(cursor, cursor)};
    }

    case Operator::PARAGRAPH_CREATE: {
        return generatParagraphCreationCommand(context);
    }

    case Operator::PARAGRAPH_JOIN: {
        auto [start, end] = SectionResolver::fromScope(context.state, {
            .scope = Scope::PARAGRAPH,
            .end_behavior = EndBehavior::STOP_BEFORE_END,
            .size = context.text_area_size
        });
        return {ModeType::TOOL_MODE, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            make_shared<SectionMoveAction>(start, end, Direction::RIGHT),
            make_shared<ParagraphJoiningAction>()
        })};
    }

    case Operator::PARAGRAPH_SPLIT: {
        return {ModeType::TOOL_MODE, make_shared<ParagraphSplittingAction>()};
    }

    case Operator::REPLACE: {
        Position cursor = context.state.getCursor().getPosition();
        return {ModeType::TOOL_MODE, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            make_shared<DeleteAction>(cursor, cursor),
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::LEFT)
        })};
    }

    case Operator::INDENT: {
        return {ModeType::TOOL_MODE, 
            make_shared<IndentAction>(context.settings.isEnabled("do_skinny_tabs")? 2 : 4)
        };
    }

    case Operator::UNINDENT: {
        return {ModeType::TOOL_MODE, 
            make_shared<UnindentAction>(context.settings.isEnabled("do_skinny_tabs")? 2 : 4)
        };
    }

    case Operator::CASE_SET_LOWER: {
        return generateCaseSetCommand(context, Case::LOWER_CASE);
    }

    case Operator::CASE_SET_UPPER: {
        return generateCaseSetCommand(context, Case::UPPER_CASE);
    }

    ///

    case Operator::FILE_ACTION: {
        return generateFileCommand(context.settings);
    }

    case Operator::UNDO: {
        return {ModeType::TOOL_MODE, make_shared<UndoAction>()};
    }


    default: {
        return emptyParse();
    }

    }

}

ParseResult CommandParser::emptyParse() {
    return {std::nullopt, std::nullopt};
}

ParseResult CommandParser::generateHint() {
    // only applies to compound commands
    switch (m_details->operator_type) {

    case Operator::MOVE_WITHIN_CHUNK:
    case Operator::MOVE_OVER_CHUNK:
    case Operator::CASE_SET_UPPER:
    case Operator::CASE_SET_LOWER: {
        return {std::nullopt, 
            make_shared<MessageAction>("Enter a scope (w,e,p,l,f) or a range identifier ( \",',(,[, {, <)!")
        };
    }
    
    case Operator::MOVE_FIND:
    case Operator::REPLACE: {
        return {std::nullopt, 
            make_shared<MessageAction>("Enter a character argument (letter, digit, special character)!")
        };

    }

    case Operator::FILE_ACTION: {
        return {std::nullopt, 
            make_shared<MessageAction>("Enter an action identifier! Refer to the help command for more info!")
        };
    }
    
    default: {
        return emptyParse();
    }
    }
}

ParseResult CommandParser::generateCharacterwiseMove(ScreenSize text_area_size) {
    return {ModeType::TOOL_MODE, 
        make_shared<CharwiseMoveAction>(text_area_size, m_details->direction)
    };
}

std::string CommandParser::getAntiDelimiter(char delimiter) {
    std::unordered_map<char, std::string> indicators = {
        {'{', "}"},
        {'<', ">"},
        {'[', "]"},
        {'(', ")"},
        {'}', "{"},
        {'>', "<"},
        {']', "["},
        {')', "("},
        {'"', "\""},
        {'\'', "'"},
    };

    if (indicators.contains(delimiter)) {
        return indicators.at(delimiter);
    }

    // already closed or symmetrical
    return "";
}

ParseResult CommandParser::generateMultiCharacterMove(ParsingContext context, EndBehavior end_behavior) {

    // range or custom delimiter
    if (!m_details->scope.has_value()) {
        auto [start, end] = SectionResolver::fromDelimiter(context.state, {
            .delimiters = std::string(1, *(m_details->argument)),
            .anti_delimiters =  getAntiDelimiter(*(m_details->argument)),
            .end_behavior = end_behavior,
            .paragraph_is_delimiter = false
        });

        return {m_details->next_mode, 
            make_shared<SectionMoveAction>(start, end, m_details->direction)
        };
    }

    ScopeSettings settings = {
        .scope = *(m_details->scope),
        .size = context.text_area_size,
        .end_behavior = end_behavior
    };

    if (settings.scope == Scope::EXPRESSION) {
        settings.delimiters = m_expression_delimiters;
    }
    else if (settings.scope == Scope::WORD) {
        settings.delimiters = m_word_delimiters;
    }

    auto [start, end] = SectionResolver::fromScope(
        context.state,
        settings
    );

    return {m_details->next_mode, make_shared<SectionMoveAction>(start, end, m_details->direction)};
}

ParseResult CommandParser::generateCaseSetCommand(ParsingContext context, Case target_case) {
    if (!m_details->scope.has_value()) {
        auto [start, end] = SectionResolver::fromDelimiter(context.state, {
            .delimiters = std::string(1, *(m_details->argument)),
            .anti_delimiters = getAntiDelimiter(*(m_details->argument)),
            .end_behavior = EndBehavior::STOP_BEFORE_END,
            .paragraph_is_delimiter = false
        });

        return {m_details->next_mode, 
            make_shared<CaseSetAction>(start, end, target_case)
        };
    }

    ScopeSettings settings = {
        .scope = *(m_details->scope),
        .size = context.text_area_size,
        .end_behavior = EndBehavior::STOP_BEFORE_END
    };

    if (settings.scope == Scope::EXPRESSION) {
        settings.delimiters = m_expression_delimiters;
    }
    else if (settings.scope == Scope::WORD) {
        settings.delimiters = m_word_delimiters;
    }

    auto [start, end] = SectionResolver::fromScope(
        context.state,
        settings
    );

    return {m_details->next_mode, make_shared<CaseSetAction>(start, end, target_case)};
}

ParseResult CommandParser::generateFileCommand(const Settings& settings) {
    SaveConfirmation confirmation = settings.isEnabled("confirm_save")?
        SaveConfirmation::YES : SaveConfirmation::NO;
        
    std::unordered_map<char, ParseResult> results = {
        {'q', {std::nullopt, make_shared<QuitAction>(QuitMode::ONLY_IF_SAVED)}},
        {'x', {std::nullopt, make_shared<QuitAction>(QuitMode::FORCE_QUIT)}},
        {'s', {std::nullopt, make_shared<SaveAction>(confirmation)}},
        {'Q', {std::nullopt, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            make_shared<SaveAction>(confirmation),
            make_shared<QuitAction>(QuitMode::ONLY_IF_SAVED)
        })}}

        //TODO: rename, open settings
    };

    if (results.contains(*(m_details->argument))) {
        return results.at(*(m_details->argument));
    }

    //return emptyParse();
    std::string message = "You entered " + std::string(1, *(m_details->argument)) + " which is not a file command i know!";
    return {std::nullopt, make_shared<MessageAction>(message)};
}

ParseResult CommandParser::generatParagraphCreationCommand(ParsingContext context) {
    auto [start, end] = SectionResolver::fromScope(context.state, {
        .scope = Scope::PARAGRAPH,
        .end_behavior = EndBehavior::STOP_BEFORE_END
    });

    if (m_details->direction == Direction::LEFT) {
        return {m_details->next_mode, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            make_shared<SectionMoveAction>(start, end, m_details->direction),
            make_shared<ParagraphSplittingAction>(),
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::UP)
        })};
    }

    return {m_details->next_mode, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
        make_shared<SectionMoveAction>(start, end, m_details->direction),
        make_shared<ParagraphSplittingAction>()
    })};

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