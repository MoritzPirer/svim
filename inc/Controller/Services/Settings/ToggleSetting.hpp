///
/// @file: ToggleSetting.hpp
/// @description: holds all information for a boolean (on/off) setting
///
/// @date: 2026-02-05
/// @author: Moritz Pirer
///

#ifndef BOOLEAN_SETTING_HPP
#define BOOLEAN_SETTING_HPP

#include <string>
#include <vector>

struct ToggleSetting {
    bool value;
    std::string display_name;
    std::vector<std::string> description;
};

#endif //BOOLEAN_SETTING_HPP
