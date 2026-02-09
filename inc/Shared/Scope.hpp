///
/// @file: Scope.hpp
/// @description: Represents the scope of an action, i.e. the section of text that should be affected
///
/// @date: 2026-01-30
/// @author: Moritz Pirer
///

#ifndef SCOPE_HPP
#define SCOPE_HPP

enum class Scope {
    WORD,
    EXPRESSION,
    LINE,
    PARAGRAPH,
    FILE
};



#endif //SCOPE_HPP
