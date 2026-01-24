///
/// @file: CharwiseMoveAction.hpp
/// @description: describes an action that moves the cursor on character or visual row
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef CHARACTERWISE_MOVE_ACTION_HPP
#define CHARACTERWISE_MOVE_ACTION_HPP

#include "Action.hpp"
#include "../Direction.hpp"

class CharwiseMoveAction: public Action {
private:
    Direction m_direction;
public:
    CharwiseMoveAction(Direction direction);
    CharwiseMoveAction(const CharwiseMoveAction&) = default;
    ~CharwiseMoveAction() = default;

    void applyTo(EditorState& state, ScreenSize size) override;
};

#endif //CHARACTERWISE_MOVE_ACTION_HPP
