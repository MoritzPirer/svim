///
/// @file: RangeSettings.hpp
/// @description: description
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef RANGE_SETTINGS_HPP
#define RANGE_SETTINGS_HPP

#include <string>
#include <optional>

#include "../Action/ActionOptions/EndBehavior.hpp"

struct RangeSettings {
    std::string delimiters;
    std::optional<std::string> anti_delimiters;
    EndBehavior end_behavior;
    bool paragraph_is_delimiter;
};

#endif //RANGE_SETTINGS_HPP
