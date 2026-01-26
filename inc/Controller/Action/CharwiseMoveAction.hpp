///
/// @file: CharwiseMoveAction.hpp
/// @description: describes an action that moves the cursor on character or visual row
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef CHARACTERWISE_MOVE_ACTION_HPP
#define CHARACTERWISE_MOVE_ACTION_HPP

#include "SizeDependantAction.hpp"
#include "../Direction.hpp"

class CharwiseMoveAction: public SizeDependantAction {
private:
    Direction m_direction;
public:
    CharwiseMoveAction(ScreenSize size, Direction direction);
    CharwiseMoveAction(const CharwiseMoveAction&) = default;
    ~CharwiseMoveAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //CHARACTERWISE_MOVE_ACTION_HPP
