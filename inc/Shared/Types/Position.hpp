///
/// @file: Position.hpp
/// @description: a position within the file
///
/// @date: 2026-01-16
/// @author: Moritz Pirer
///

#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>

struct Position {
    int row;
    int column;    

    Position(int r = 0, int c = 0): row{r}, column{c} {}

    std::string format() const {
        return "("
            + std::to_string(row)
            + "/"
            + std::to_string(column)
            + ")";
        }
};

#endif //POSITION_HPP
