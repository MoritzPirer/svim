///
/// @file: CharwiseMoveAction.hpp
/// @description: Moves the cursor one character over or one line up / down
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef CHARACTERWISE_MOVE_ACTION_HPP
#define CHARACTERWISE_MOVE_ACTION_HPP

#include "../Action.hpp"
#include "../../../Shared/Types/Direction.hpp"

class CharwiseMoveAction: public Action {
private:
    ScreenSize m_size;
    Direction m_direction;
public:
    CharwiseMoveAction(ScreenSize size, Direction direction);
    CharwiseMoveAction(const CharwiseMoveAction&) = default;
    ~CharwiseMoveAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //CHARACTERWISE_MOVE_ACTION_HPP
