///
/// @file: Position.hpp
/// @description: description
///
/// @date: 2026-01-16
/// @author: Moritz Pirer
///

#ifndef POSITION_HPP
#define POSITION_HPP

#include <stddef.h>

struct Position {
    size_t row;
    size_t column;    

    Position(size_t r = 0, size_t c = 0): row{r}, column{c} {}
};

#endif //POSITION_HPP
