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

#include "OperatorType.hpp"
#include "../../Shared/Scope.hpp"
#include "../../Shared/ParseResult.hpp"
#include "../../Shared/Destination.hpp"
#include "../Settings/Settings.hpp"

class CommandParser {
private:
    std::optional<OperatorType> m_operator_type;
    std::optional<Direction> m_direction;
    std::optional<Destination> m_destination;
    std::optional<char> m_argument;
    std::optional<char> m_range;
    std::optional<Scope> m_scope;
    std::optional<ModeType> m_next_mode;
    bool m_is_complete;

    std::string m_expression_delimiters = " \t";
    std::string m_word_delimiters = " \t!\"$%&/()=?[]|{}`+*#'-_.:,;<>";

    ParseResult emptyParse();

    ParseResult generateCharacterwiseMove(ScreenSize text_area_size);
    ParseResult generateMoveWithinChunk(ScreenSize text_area_size, ModeType mode);
    ParseResult generateMoveOverChunk(ScreenSize text_area_size);
    
    ParseResult generateActions(ScreenSize text_area_size);
    void parseAsOperator(char input);

    ParseResult parseAsMoveInput(char input, ScreenSize text_area_size);

    bool operatorExpectsScopeOrRange();
    void parseAsScopeOrRange(char input);
    ParseResult tryGenerateHint();
public:
    CommandParser() = default;
    CommandParser(const CommandParser&) = default;
    ~CommandParser() = default;

    ParseResult parseInput(char input, ScreenSize text_area_size, const Settings& settings);

};

#endif //COMMAND_PARSER_HPP
