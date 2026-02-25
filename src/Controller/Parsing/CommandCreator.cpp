#include <unordered_map>

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
#include "../../../inc/Controller/Actions/System/CopyAction.hpp"

#include "../../../inc/Controller/Actions/Editing/InsertAction.hpp"
#include "../../../inc/Controller/Actions/Editing/DeleteAction.hpp"
#include "../../../inc/Controller/Actions/Editing/IndentAction.hpp"
#include "../../../inc/Controller/Actions/Editing/UnindentAction.hpp"
#include "../../../inc/Controller/Actions/Editing/CaseSetAction.hpp"

#include "../../../inc/Controller/Actions/Structure/ParagraphSplittingAction.hpp"
#include "../../../inc/Controller/Actions/Structure/ParagraphJoiningAction.hpp"

#include "../../../inc/Controller/Parsing/SpanResolver.hpp"

using std::make_shared;
typedef std::vector<std::shared_ptr<Action>> ActionList;

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

    case Operator::MOVE_TO_FIND: {
        return generateSpanMove(*details, context, EndBehavior::STOP_ON_END);
    }

    case Operator::MOVE_TO_END: {
        return generateSpanMove(*details, context, EndBehavior::STOP_BEFORE_END);
    }

    case Operator::MOVE_TO_NEXT: {
        return generateSpanMove(*details, context, EndBehavior::STOP_AFTER_END);
    }

    /// Editing

    case Operator::DELETE_SINGLE: {
        Position cursor = state.getCursor().getPosition();
        return {details->next_mode, make_shared<DeleteAction>(cursor, cursor, cursor)};
    }

    case Operator::DELETE_WITHIN: {
        return generateDeleteWithinCommand(*details, context);
    }

    case Operator::DELETE_UNTIL: {
        return generateDeleteUntilCommand(*details, context);
    }

    case Operator::PARAGRAPH_CREATE: {
        return generatParagraphCreationCommand(*details, context);
    }

    case Operator::PARAGRAPH_JOIN: {
        auto [start, end] = SpanResolver::fromScope(state, {
            .scope = Scope::PARAGRAPH,
            .size = context.text_area_size,
            .end_behavior = EndBehavior::STOP_BEFORE_END
        });
        return {ModeType::TOOL_MODE, make_shared<CompoundAction>(ActionList{
            make_shared<SpanMoveAction>(start, end, Direction::RIGHT),
            make_shared<ParagraphJoiningAction>(context.state.getCursor().getPosition())
        })};
    }

    case Operator::PARAGRAPH_SPLIT: {
        return {ModeType::TOOL_MODE, std::make_shared<ParagraphSplittingAction>(context.state.getCursor().getPosition())};
    }

    case Operator::REPLACE: {
        Position cursor = state.getCursor().getPosition();
        return {ModeType::TOOL_MODE, make_shared<CompoundAction>(ActionList{
            make_shared<DeleteAction>(cursor, cursor, cursor),
            make_shared<InsertAction>(std::vector<std::string>{std::string(1, *(details->argument))}, cursor),
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

    case Operator::COPY_WITHIN: {
        return generateCopyWithinCommand(*details, context);
    }
    case Operator::COPY_UNTIL: {
        return generateCopyUntilCommand(*details, context);
    }

    case Operator::PASTE: {
        return gerneratePasteCommand(*details, context);
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
    std::string scope_range_hint = "Enter a scope or range!";
    
    std::unordered_map<Operator, std::string> hints = {
        {Operator::COPY_UNTIL, "Enter the end of the section to copy!"},
        {Operator::COPY_WITHIN, "Enter a scope or range to copy!"},
        {Operator::DELETE_UNTIL, "Enter the end of the section to delete!"},
        {Operator::DELETE_WITHIN, "Enter a scope or range to delete!"},
        {Operator::REPLACE, "Enter a character to replace the selected character!"},
        {Operator::MOVE_TO_END, scope_range_hint},
        {Operator::MOVE_TO_NEXT, scope_range_hint},
        {Operator::MOVE_TO_FIND, "Enter a character to find!"},
        {Operator::CASE_SET_UPPER, "Enter a scope or range to set to uppercase!"},
        {Operator::CASE_SET_LOWER, "Enter a scope or range to set to lowercase!"},
        {Operator::FILE_ACTION, "x to force quit, q to quit safely, Q to save and quit, s to save"}
    }; 
    
    if (hints.contains(details.operator_type)) {
        return {std::nullopt, make_shared<NotifyAction>(hints.at(details.operator_type))};
    }
    
    return emptyParse();
}

ParseResult CommandCreator::generateCharacterwiseMove(CommandDetails details, ScreenSize text_area_size) {
    return {ModeType::TOOL_MODE, make_shared<CharwiseMoveAction>(text_area_size, *details.direction)};
}

std::string CommandCreator::getAntiDelimiter(char delimiter) {
    std::unordered_map<char, std::string> indicators = {
        {'{', "}"},
        {'}', "{"},
        {'<', ">"},
        {'>', "<"},
        {'[', "]"},
        {'(', ")"},
        {')', "("},
        {']', "["},
        {'"', "\""},
        {'\'', "'"},
    };

    if (indicators.contains(delimiter)) {
        return indicators.at(delimiter);
    }

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

        return {details.next_mode, make_shared<SpanMoveAction>(start, end, *details.direction)};
    }

    // scope given
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

    auto [start, end] = SpanResolver::fromScope(context.state, settings);

    return {details.next_mode, make_shared<SpanMoveAction>(start, end, *details.direction)};
}

ParseResult CommandCreator::generateCaseSetCommand(CommandDetails details, ParsingContext context, Case target_case) {
    // range or delimiter given
    if (!details.scope.has_value()) {
        auto [start, end] = SpanResolver::fromDelimiter(context.state, {
            .delimiters = std::string(1, *(details.argument)),
            .anti_delimiters = getAntiDelimiter(*(details.argument)),
            .end_behavior = EndBehavior::STOP_BEFORE_END,
            .paragraph_is_delimiter = false
        });

        return {details.next_mode, make_shared<CaseSetAction>(start, end, target_case)};
    }

    // scope given
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

    auto [start, end] = SpanResolver::fromScope(context.state, settings);

    return {details.next_mode, make_shared<CaseSetAction>(start, end, target_case)};
}

ParseResult CommandCreator::generateFileCommand(CommandDetails details, const Settings& settings) {
    SaveConfirmation confirmation = settings.isEnabled("confirm_save")?
        SaveConfirmation::YES : SaveConfirmation::NO;
        
    std::unordered_map<char, ParseResult> results = {
        {'q', {std::nullopt, make_shared<QuitAction>(QuitMode::ONLY_IF_SAVED)}},
        {'x', {std::nullopt, make_shared<QuitAction>(QuitMode::FORCE_QUIT)}},
        {'s', {std::nullopt, make_shared<SaveAction>(confirmation)}},
        {'Q', {std::nullopt, make_shared<CompoundAction>(ActionList{
            make_shared<SaveAction>(confirmation),
            make_shared<QuitAction>(QuitMode::ONLY_IF_SAVED)
        })}}
    };

    if (results.contains(*(details.argument))) {
        return results.at(*(details.argument));
    }

    std::string message = std::string(1, *(details.argument)) + " is not a known file command!";
    return {std::nullopt, make_shared<NotifyAction>(message)};
}

ParseResult CommandCreator::generatParagraphCreationCommand(CommandDetails details, ParsingContext context) {
    auto [start, end] = SpanResolver::fromScope(context.state, {
        .scope = Scope::PARAGRAPH,
        .end_behavior = EndBehavior::STOP_BEFORE_END
    });

    if (*details.direction == Direction::LEFT) {
        return {details.next_mode, make_shared<CompoundAction>(ActionList{
            make_shared<SpanMoveAction>(start, end, *details.direction),
            make_shared<ParagraphSplittingAction>(start),
        })};
    }
    
    return {details.next_mode, make_shared<CompoundAction>(ActionList{
        make_shared<SpanMoveAction>(start, end, *details.direction),
        make_shared<ParagraphSplittingAction>(end),
        make_shared<CharwiseMoveAction>(context.text_area_size, Direction::RIGHT)
    })};
}

ParseResult CommandCreator::generateDeleteWithinCommand(CommandDetails details, ParsingContext context) {
    Position cursor = context.state.getCursor().getPosition();

    // range or custom delimiter
    if (!details.scope.has_value()) {
        auto [start, end] = SpanResolver::fromDelimiter(context.state, {
            .delimiters = std::string(1, *(details.argument)),
            .anti_delimiters = getAntiDelimiter(*(details.argument)),
            .end_behavior = EndBehavior::STOP_BEFORE_END,
            .paragraph_is_delimiter = false
        });

        return {details.next_mode, make_shared<DeleteAction>(start, end, cursor)};
    }

    // Scope given
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

    auto [start, end] = SpanResolver::fromScope(context.state, settings);

    return {details.next_mode, make_shared<DeleteAction>(start, end, cursor)};

}

ParseResult CommandCreator::generateDeleteUntilCommand(CommandDetails details, ParsingContext context) {
    Position cursor = context.state.getCursor().getPosition();

    auto [dummy, end] = SpanResolver::fromDelimiter(context.state, {
        .delimiters = getAntiDelimiter(*(details.argument)),
        .anti_delimiters = std::string(1, *(details.argument)),
        .end_behavior = EndBehavior::STOP_BEFORE_END,
        .paragraph_is_delimiter = false
    });

    return {details.next_mode, make_shared<DeleteAction>(cursor, end, cursor)};
}

ParseResult CommandCreator::generateCopyWithinCommand(CommandDetails details, ParsingContext context) {
    // range or custom delimiter
    if (!details.scope.has_value()) {
        auto [start, end] = SpanResolver::fromDelimiter(context.state, {
            .delimiters = std::string(1, *(details.argument)),
            .anti_delimiters = getAntiDelimiter(*(details.argument)),
            .end_behavior = EndBehavior::STOP_BEFORE_END,
            .paragraph_is_delimiter = false
        });

        return {ModeType::TOOL_MODE, make_shared<CompoundAction>(ActionList{
            make_shared<CopyAction>(start, end, CopyType::INLINE),
            make_shared<NotifyAction>("Copied content to clipboard!")
            })
        };
    }

    // Scope given
    ScopeSettings settings = {
        .scope = *(details.scope),
        .size = context.text_area_size,
        .end_behavior = EndBehavior::STOP_BEFORE_END
    };

    CopyType type = CopyType::INLINE;
    switch (settings.scope) {
        case Scope::FILE:
        case Scope::PARAGRAPH: {
            type = CopyType::BLOCK;
            break;
        }
        case Scope::EXPRESSION: {
            settings.delimiters = m_expression_delimiters;
            break;
        }
        case Scope::WORD: {
            settings.delimiters = m_word_delimiters;
            break;
        }
        default: {
            break;
        }
    }

    auto [start, end] = SpanResolver::fromScope(context.state, settings);
    if (static_cast<size_t>(end.column) == context.state.getParagraph(end.row).length()
        && end.column != 0) {
        end.column--;
    }

    return {ModeType::TOOL_MODE, make_shared<CompoundAction>(ActionList{
        make_shared<CopyAction>(start, end, type),
        make_shared<NotifyAction>("Copied content to clipboard!")
    })};
}

ParseResult CommandCreator::generateCopyUntilCommand(CommandDetails details, ParsingContext context) {
    Position cursor = context.state.getCursor().getPosition();

    auto [dummy, end] = SpanResolver::fromDelimiter(context.state, {
        .delimiters = getAntiDelimiter(*(details.argument)),
        .anti_delimiters = std::string(1, *(details.argument)),
        .end_behavior = EndBehavior::STOP_BEFORE_END,
        .paragraph_is_delimiter = false
    });

    return {std::nullopt, make_shared<CopyAction>(cursor, end, CopyType::INLINE)};
}

ParseResult CommandCreator::gerneratePasteCommand(CommandDetails details, ParsingContext context) {
    std::optional<Clipboard> clipboard = context.state.getClipboard();
    Position cursor = context.state.getCursor().getPosition();

    if (!clipboard.has_value()) {
        return {std::nullopt, make_shared<NotifyAction>("Nothing to paste! Copy text using y or Y.")};
    }

    if (clipboard->type == CopyType::INLINE) {
        if (details.direction == Direction::LEFT) {
            return {std::nullopt, make_shared<InsertAction>(clipboard->content, cursor, true)};
        }
        
        //paste after cursor
        if (static_cast<size_t>(cursor.column) < context.state.getParagraph(cursor.row).length()) {
            cursor.column++;
        }

        return {std::nullopt, make_shared<InsertAction>(clipboard->content, cursor, true)};
    }
    
    //paste full line above / below cursor
    return emptyParse();
}