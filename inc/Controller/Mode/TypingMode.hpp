///
/// @file: TypingMode.hpp
/// @description: description
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef TYPING_MODE_HPP
#define TYPING_MODE_HPP

#include "EditorMode.hpp"

class TypingMode: public EditorMode {
private:

public:
    TypingMode() = default;
    TypingMode(const TypingMode&) = default;
    ~TypingMode() = default;

    std::pair<ModeType, std::shared_ptr<Action>> parseInput(int input, ScreenSize size) override;
    std::string getModeLabel() const override { return "TYPING"; }
};

#endif //TYPING_MODE_HPP
