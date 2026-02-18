///
/// @file: TypingMode.hpp
/// @description: the mode that is primarily used for adding and deleting
///     characters in the classic way
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef TYPING_MODE_HPP
#define TYPING_MODE_HPP

#include "EditorMode.hpp"
#include "../Parsing/ParsingContext.hpp"

class TypingMode: public EditorMode {
private:
    ParseResult parseSpecialKey(SpecialKey key, ParsingContext context);

public:
    TypingMode() = default;
    TypingMode(const TypingMode&) = default;
    ~TypingMode() = default;

    ParseResult parseInput(Input input, ParsingContext context) override;

    std::string getModeLabel() const override { return "TYPING MODE"; }
};

#endif //TYPING_MODE_HPP
