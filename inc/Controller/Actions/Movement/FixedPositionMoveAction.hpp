///
/// @file: FixedPositionMoveAction.hpp
/// @description: describes an action that moves the cursor to a given position
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef FIXED_POSITION_MOVE_ACTION
#define FIXED_POSITION_MOVE_ACTION

#include "../Action.hpp"
#include "../../../Shared/Types/Direction.hpp"

class FixedPositionMoveAction: public Action {
private:
    Position m_target_position;

public:
    FixedPositionMoveAction(Position target_position);
    FixedPositionMoveAction(const FixedPositionMoveAction&) = default;
    ~FixedPositionMoveAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //FIXED_POSITION_MOVE_ACTION
