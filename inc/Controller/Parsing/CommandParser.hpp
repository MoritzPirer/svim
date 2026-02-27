///
/// @file: CommandParser.hpp
/// @description: Parses user input into CommandDetails 
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <optional>

#include "../Actions/Options/Case.hpp"
#include "../Actions/Options/EndBehavior.hpp"
#include "../Services/Settings/Settings.hpp"
#include "../../Shared/DataFlow/ParseResult.hpp"
#include "../../Shared/Types/Scope.hpp"
#include "CommandDetails.hpp"
#include "CommandCreator.hpp"
#include "Operator.hpp"
#include "ParsingContext.hpp"

class CommandParser {
private:
    std::optional<CommandDetails> m_details;
    CommandCreator m_creator;

    void parseAsOperator(char input);
    void parseAsParameter(char input);

    bool isRangeIndicator(char c);
    std::optional<Scope> charToScope(char c);

public:
    CommandParser() = default;
    CommandParser(const CommandParser&) = default;
    ~CommandParser() = default;

    ParseResult parseInput(char input, ParsingContext context);

};

#endif //COMMAND_PARSER_HPP
