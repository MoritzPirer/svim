///
/// @file: SectionMoveAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef DELIMITER_MOVE_ACTION_HPP
#define DELIMITER_MOVE_ACTION_HPP

#include "RangedAction.hpp"

class SectionMoveAction: public RangedAction {
private:
    Direction m_direction;
public:
    SectionMoveAction(Position start, Position end, Direction direction);
    SectionMoveAction(const SectionMoveAction&) = default;
    ~SectionMoveAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //DELIMITER_MOVE_ACTION_HPP
