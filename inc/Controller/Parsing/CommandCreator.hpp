///
/// @file: CommandCreator.hpp
/// @description: Creates a ParseResult based on a CommandDetails struct populated by user input
///
/// @date: 2026-02-18
/// @author: Moritz Pirer
///

#ifndef COMMAND_CREATOR_HPP
#define COMMAND_CREATOR_HPP

#include "../../Shared/DataFlow/ParseResult.hpp"
#include "ParsingContext.hpp"
#include "../Actions/Options/Case.hpp"
#include "../Actions/Options/EndBehavior.hpp"
#include "CommandDetails.hpp"

class CommandCreator {
private:

    std::string m_expression_delimiters = " \t";
    std::string m_word_delimiters = " \t!\"$%&/()=?[]|{}`+*#'-_.:,;<>";

    ParseResult emptyParse();

    ParseResult generateCharacterwiseMove(CommandDetails details, ScreenSize text_area_size);
    ParseResult generateSpanMove(CommandDetails details, ParsingContext context, EndBehavior end_behavior);

    ParseResult generateCaseSetCommand(CommandDetails details, ParsingContext context, Case target_case);
    ParseResult generateFileCommand(CommandDetails details, const Settings& settings);
    ParseResult generatParagraphCreationCommand(CommandDetails details, ParsingContext context);

    ParseResult generateHint(CommandDetails details);
    std::string getAntiDelimiter(char delimiter);

public:
    CommandCreator() = default;
    CommandCreator(const CommandCreator&) = default;
    ~CommandCreator() = default;

    ParseResult generateActions(std::optional<CommandDetails> details, ParsingContext context);
};

#endif //COMMAND_CREATOR_HPP
