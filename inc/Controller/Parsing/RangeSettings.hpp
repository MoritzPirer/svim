///
/// @file: RangeSettings.hpp
/// @description: Settings needed to generate a Span based on delimiters
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef RANGE_SETTINGS_HPP
#define RANGE_SETTINGS_HPP

#include <string>
#include <optional>

#include "../Actions/Options/EndBehavior.hpp"

struct RangeSettings {
    std::string delimiters; // list of characters that end the range

    // list of characters that counteract the delimiters, e.g. '}' if '{' is the delimiter
    std::optional<std::string> anti_delimiters; 
    EndBehavior end_behavior;

    // end the search upon reaching the end of a paragraph
    bool paragraph_is_delimiter;
};

#endif //RANGE_SETTINGS_HPP
