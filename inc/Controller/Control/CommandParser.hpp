///
/// @file: CommandParser.hpp
/// @description: description
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <optional>

#include "../Action/ActionOptions/Case.hpp"
#include "../Action/ActionOptions/EndBehavior.hpp"
#include "../Settings/Settings.hpp"
#include "../../Shared/Destination.hpp"
#include "../../Shared/ParseResult.hpp"
#include "../../Shared/Scope.hpp"
#include "CommandDetails.hpp"
#include "Operator.hpp"
#include "ParsingContext.hpp"

class CommandParser {
private:
    std::optional<CommandDetails> m_details;

    std::string m_expression_delimiters = " \t";
    std::string m_word_delimiters = " \t!\"$%&/()=?[]|{}`+*#'-_.:,;<>";

    ParseResult emptyParse();

    ParseResult generateCharacterwiseMove(ScreenSize text_area_size);
    ParseResult generateMultiCharacterMove(ParsingContext context, EndBehavior end_behavior);
    
    ParseResult generateCaseSetCommand(ParsingContext context, Case target_case);
    ParseResult generateFileCommand(const Settings& settings);
    ParseResult generatParagraphCreationCommand(ParsingContext context);
    
    ParseResult generateHint();
    ParseResult generateActions(ParsingContext context);
    
    
    void parseAsOperator(char input);
    void parseAsParameter(char input);

    bool isRangeIndicator(char c);
    char getOpeningRangeIndicator(char range_indicator);
    char getClosingRangeIndicator(char range_indicator);
    std::string getAntiDelimiter(char delimiter);
    std::optional<Scope> charToScope(char c);

public:
    CommandParser() = default;
    CommandParser(const CommandParser&) = default;
    ~CommandParser() = default;

    ParseResult parseInput(char input, ParsingContext context);

};

#endif //COMMAND_PARSER_HPP
