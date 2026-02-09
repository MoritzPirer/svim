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
    bool m_is_complete_command;

    std::string m_expression_delimiters = " \t";
    std::string m_word_delimiters = " \t!\"$%&/()=?[]|{}`+*#'-_.:,;<>";

    ParseResult emptyParse();

    ParseResult generateMultiCharacterMove(ScreenSize text_area_size, EndBehavior end_behaviour);
    ParseResult generateCharacterwiseMove(ScreenSize text_area_size);
    ParseResult generateMoveWithinChunk(ScreenSize text_area_size, ModeType mode);
    ParseResult generateMoveOverChunk(ScreenSize text_area_size);
    
    ParseResult generateActions(ScreenSize text_area_size);
    
    
    void parseAsOperator(char input);
    void parseAsArgument(char input);

    bool operatorExpectsScopeOrRange();
    void parseAsScopeOrRange(char input);

    void parseAsParameter(char input);

    bool isRangeIndicator(char c);

    std::optional<Scope> charToScope(char c);
    ParseResult tryGenerateHint();
public:
    CommandParser() = default;
    CommandParser(const CommandParser&) = default;
    ~CommandParser() = default;

    ParseResult parseInput(char input, ScreenSize text_area_size, const Settings& settings);

};

#endif //COMMAND_PARSER_HPP
