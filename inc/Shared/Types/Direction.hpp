///
/// @file: Direction.hpp
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef DIRECTION_HPP
#define DIRECTION_HPP

enum class Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN
};

Direction getOppositeDirection(Direction direction);

#endif //DIRECTION_HPP
