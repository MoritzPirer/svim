#include "../../../inc/Controller/Parsing/CommandCreator.hpp"
#include "../../../inc/Controller/Parsing/CommandDetails.hpp"
#include "../../../inc/Controller/Parsing/ParsingContext.hpp"
#include "../../../inc/Controller/Services/Settings/Settings.hpp"

#include "../../../inc/Controller/Actions/Movement/CharwiseMoveAction.hpp"
#include "../../../inc/Controller/Actions/Movement/FixedPositionMoveAction.hpp"
#include "../../../inc/Controller/Actions/Movement/SpanMoveAction.hpp"

#include "../../../inc/Controller/Actions/System/CompoundAction.hpp"
#include "../../../inc/Controller/Actions/System/SaveAction.hpp"
#include "../../../inc/Controller/Actions/System/QuitAction.hpp"
#include "../../../inc/Controller/Actions/System/NotifyAction.hpp"
#include "../../../inc/Controller/Actions/System/UndoAction.hpp"
#include "../../../inc/Controller/Actions/System/RedoAction.hpp"

#include "../../../inc/Controller/Actions/Editing/InsertAction.hpp"
#include "../../../inc/Controller/Actions/Editing/DeleteAction.hpp"
#include "../../../inc/Controller/Actions/Editing/IndentAction.hpp"
#include "../../../inc/Controller/Actions/Editing/UnindentAction.hpp"
#include "../../../inc/Controller/Actions/Editing/CaseSetAction.hpp"

#include "../../../inc/Controller/Actions/Structure/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Actions/Structure/ParagraphJoiningAction.hpp"

#include "../../../inc/Controller/Parsing/SpanResolver.hpp"

using std::make_shared;

ParseResult CommandCreator::generateActions(std::optional<CommandDetails> details, ParsingContext context) {
    if (!details.has_value() ) {
        return emptyParse();
    }

    if (!details->is_complete) {
        return generateHint(*details);
    }

    EditorState& state = context.state;

    switch (details->operator_type) {
    case Operator::SWITCH_MODE: {
        return {ModeType::TYPING_MODE, std::nullopt};
    }

    /// Movement

    case Operator::MOVE_BY_CHARACTER: {
        return generateCharacterwiseMove(*details, context.text_area_size);
    }

    case Operator::MOVE_FIND: {
        return generateSpanMove(*details, context, EndBehavior::STOP_ON_END);
    }

    case Operator::MOVE_WITHIN_CHUNK: {
        return generateSpanMove(*details, context, EndBehavior::STOP_BEFORE_END);
    }

    case Operator::MOVE_OVER_CHUNK: {
        return generateSpanMove(*details, context, EndBehavior::STOP_AFTER_END);
    }

    /// Editing

    case Operator::ERASE: {
        Position cursor = state.getCursor().getPosition();
        return {details->next_mode, make_shared<DeleteAction>(cursor, cursor)};
    }

    case Operator::PARAGRAPH_CREATE: {
        return generatParagraphCreationCommand(*details, context);
    }

    case Operator::PARAGRAPH_JOIN: {
        auto [start, end] = SpanResolver::fromScope(state, {
            .scope = Scope::PARAGRAPH,
            .end_behavior = EndBehavior::STOP_BEFORE_END,
            .size = context.text_area_size
        });
        return {ModeType::TOOL_MODE, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            make_shared<SpanMoveAction>(start, end, Direction::RIGHT),
            make_shared<ParagraphJoiningAction>(context.state.getCursor().getPosition())
        })};
    }

    case Operator::PARAGRAPH_SPLIT: {
        return {ModeType::TOOL_MODE, std::make_shared<ParagraphSplittingAction>(context.state.getCursor().getPosition())};
    }

    case Operator::REPLACE: {
        Position cursor = state.getCursor().getPosition();
        return {ModeType::TOOL_MODE, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            make_shared<DeleteAction>(cursor, cursor),
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::LEFT)
        })};
    }

    case Operator::INDENT: {
        return {ModeType::TOOL_MODE, 
            make_shared<IndentAction>(state.getCursor().getRow(), context.settings.getTabWidth())
        };
    }

    case Operator::UNINDENT: {
        return {ModeType::TOOL_MODE, 
            make_shared<UnindentAction>(context.state.getCursor().getRow(), context.settings.getTabWidth())
        };
    }

    case Operator::CASE_SET_LOWER: {
        return generateCaseSetCommand(*details, context, Case::LOWER_CASE);
    }

    case Operator::CASE_SET_UPPER: {
        return generateCaseSetCommand(*details, context, Case::UPPER_CASE);
    }

    ///

    case Operator::FILE_ACTION: {
        return generateFileCommand(*details, context.settings);
    }

    case Operator::UNDO: {
        return {ModeType::TOOL_MODE, make_shared<UndoAction>()};
    }

    case Operator::REDO: {
        return {ModeType::TOOL_MODE, make_shared<RedoAction>()};
    }

    default: {
        return emptyParse();
    }

    }

}

ParseResult CommandCreator::emptyParse() {
    return {std::nullopt, std::nullopt};
}

ParseResult CommandCreator::generateHint(CommandDetails details) {
    // only applies to compound commands
    switch (details.operator_type) {

    case Operator::MOVE_WITHIN_CHUNK:
    case Operator::MOVE_OVER_CHUNK:
    case Operator::CASE_SET_UPPER:
    case Operator::CASE_SET_LOWER: {
        return {std::nullopt, 
            make_shared<NotifyAction>("Enter a scope (w,e,p,l,f) or a range identifier ( \",',(,[, {, <)!")
        };
    }
    
    case Operator::MOVE_FIND:
    case Operator::REPLACE: {
        return {std::nullopt, 
            make_shared<NotifyAction>("Enter a character argument (letter, digit, special character)!")
        };

    }

    case Operator::FILE_ACTION: {
        return {std::nullopt, 
            make_shared<NotifyAction>("Enter an action identifier! Refer to the help command for more info!")
        };
    }
    
    default: {
        return emptyParse();
    }
    }
}

ParseResult CommandCreator::generateCharacterwiseMove(CommandDetails details, ScreenSize text_area_size) {
    return {ModeType::TOOL_MODE, 
        make_shared<CharwiseMoveAction>(text_area_size, details.direction)
    };
}

std::string CommandCreator::getAntiDelimiter(char delimiter) {
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

ParseResult CommandCreator::generateSpanMove(CommandDetails details, ParsingContext context, EndBehavior end_behavior) {

    // range or custom delimiter
    if (!details.scope.has_value()) {
        auto [start, end] = SpanResolver::fromDelimiter(context.state, {
            .delimiters = std::string(1, *(details.argument)),
            .anti_delimiters =  getAntiDelimiter(*(details.argument)),
            .end_behavior = end_behavior,
            .paragraph_is_delimiter = false
        });

        return {details.next_mode, 
            make_shared<SpanMoveAction>(start, end, details.direction)
        };
    }

    ScopeSettings settings = {
        .scope = *(details.scope),
        .size = context.text_area_size,
        .end_behavior = end_behavior
    };

    if (settings.scope == Scope::EXPRESSION) {
        settings.delimiters = m_expression_delimiters;
    }
    else if (settings.scope == Scope::WORD) {
        settings.delimiters = m_word_delimiters;
    }

    auto [start, end] = SpanResolver::fromScope(
        context.state,
        settings
    );

    return {details.next_mode, make_shared<SpanMoveAction>(start, end, details.direction)};
}

ParseResult CommandCreator::generateCaseSetCommand(CommandDetails details, ParsingContext context, Case target_case) {
    if (!details.scope.has_value()) {
        auto [start, end] = SpanResolver::fromDelimiter(context.state, {
            .delimiters = std::string(1, *(details.argument)),
            .anti_delimiters = getAntiDelimiter(*(details.argument)),
            .end_behavior = EndBehavior::STOP_BEFORE_END,
            .paragraph_is_delimiter = false
        });

        return {details.next_mode, 
            make_shared<CaseSetAction>(start, end, target_case)
        };
    }

    ScopeSettings settings = {
        .scope = *(details.scope),
        .size = context.text_area_size,
        .end_behavior = EndBehavior::STOP_BEFORE_END
    };

    if (settings.scope == Scope::EXPRESSION) {
        settings.delimiters = m_expression_delimiters;
    }
    else if (settings.scope == Scope::WORD) {
        settings.delimiters = m_word_delimiters;
    }

    auto [start, end] = SpanResolver::fromScope(
        context.state,
        settings
    );

    return {details.next_mode, make_shared<CaseSetAction>(start, end, target_case)};
}

ParseResult CommandCreator::generateFileCommand(CommandDetails details, const Settings& settings) {
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

    if (results.contains(*(details.argument))) {
        return results.at(*(details.argument));
    }

    //return emptyParse();
    std::string message = "You entered " + std::string(1, *(details.argument)) + " which is not a file command i know!";
    return {std::nullopt, make_shared<NotifyAction>(message)};
}

ParseResult CommandCreator::generatParagraphCreationCommand(CommandDetails details, ParsingContext context) {
    auto [start, end] = SpanResolver::fromScope(context.state, {
        .scope = Scope::PARAGRAPH,
        .end_behavior = EndBehavior::STOP_BEFORE_END
    });

    if (details.direction == Direction::LEFT) {
        return {details.next_mode, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
            make_shared<SpanMoveAction>(start, end, details.direction),
            make_shared<ParagraphSplittingAction>(context.state.getCursor().getPosition()),
            make_shared<CharwiseMoveAction>(context.text_area_size, Direction::UP)
        })};
    }

    return {details.next_mode, make_shared<CompoundAction>(std::vector<std::shared_ptr<Action>>{
        make_shared<SpanMoveAction>(start, end, details.direction),
        make_shared<ParagraphSplittingAction>(context.state.getCursor().getPosition()),
    })};
}