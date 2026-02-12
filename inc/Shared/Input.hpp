///
/// @file: Input.hpp
/// @description: input given by the user
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef INPUT_HPP
#define INPUT_HPP

#include <optional>

#include "Position.hpp"
#include "SpecialKey.hpp"

struct Input {
    std::optional<Position> mouse_position;
    std::optional<SpecialKey> special_key;
    std::optional<char> standard_input;

    Input(Position position):
        mouse_position{position},
        special_key{std::nullopt},
        standard_input{std::nullopt} {}

    Input(SpecialKey key):
        mouse_position{std::nullopt},
        special_key{key},
        standard_input{std::nullopt} {}

    Input(char input):
        mouse_position{std::nullopt},
        special_key{std::nullopt},
        standard_input{input} {}
    
    Input():
        mouse_position{std::nullopt},
        special_key{std::nullopt},
        standard_input{std::nullopt} {}
    
};

#endif //INPUT_HPP
