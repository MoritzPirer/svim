///
/// @file: ScopeSettings.hpp
/// @description: description
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef SCOPE_SETTINGS_HPP
#define SCOPE_SETTINGS_HPP

#include <string>
#include "../../Shared/ScreenSize.hpp"
#include "../../Shared/Scope.hpp"
#include "../Action/ActionOptions/EndBehavior.hpp"


struct ScopeSettings {
    Scope scope;
    ScreenSize size;
    EndBehavior end_behavior;
    std::string delimiters;
};


#endif //SCOPE_SETTINGS_HPP
