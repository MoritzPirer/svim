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

#include "Operator.hpp"
#include "CommandDetails.hpp"
#include "../Action/ActionOptions/EndBehavior.hpp"
#include "../../Shared/Scope.hpp"
#include "../../Shared/ParseResult.hpp"
#include "../../Shared/Destination.hpp"
#include "../Settings/Settings.hpp"

class CommandParser {
private:
    std::optional<CommandDetails> m_details;

    std::string m_expression_delimiters = " \t";
    std::string m_word_delimiters = " \t!\"$%&/()=?[]|{}`+*#'-_.:,;<>";

    ParseResult emptyParse();

    ParseResult generateCharacterwiseMove(ScreenSize text_area_size);
    ParseResult generateMultiCharacterMove(ScreenSize text_area_size, EndBehavior end_behaviour);

    ParseResult generateFileCommand(const Settings& settings);
    ParseResult generatParagraphCreationCommand(ScreenSize text_area_size);
    
    ParseResult generateHint();
    ParseResult generateActions(ScreenSize text_area_size, const Settings& settings);
    
    
    void parseAsOperator(char input);
    void parseAsParameter(char input);

    bool isRangeIndicator(char c);
    char getOpeningRangeIndicator(char range_indicator);
    char getClosingRangeIndicator(char range_indicator);
    std::optional<Scope> charToScope(char c);

public:
    CommandParser() = default;
    CommandParser(const CommandParser&) = default;
    ~CommandParser() = default;

    ParseResult parseInput(char input, ScreenSize text_area_size, const Settings& settings);

};

#endif //COMMAND_PARSER_HPP
