///
/// @file: ScopeSettings.hpp
/// @description: Settings needed to generate a Span based on a scope
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef SCOPE_SETTINGS_HPP
#define SCOPE_SETTINGS_HPP

#include <string>
#include "../../Shared/Types/ScreenSize.hpp"
#include "../../Shared/Types/Scope.hpp"
#include "../Actions/Options/EndBehavior.hpp"

struct ScopeSettings {
    Scope scope;
    ScreenSize size;
    EndBehavior end_behavior;
    std::string delimiters;
};


#endif //SCOPE_SETTINGS_HPP
