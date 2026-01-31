#include "../../inc/Shared/Direction.hpp"

Direction getOppositeDirection(Direction direction) {
    switch (direction) {
        case Direction::FORWARD:
            return Direction::BACKWARD; 

        case Direction::BACKWARD:
            return Direction::FORWARD;

        case Direction::UP:
            return Direction::DOWN;

        case Direction::DOWN:
            return Direction::UP;
        default:
            break;
    }
}