///
/// @file: DelimiterMoveAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef DELIMITER_MOVE_ACTION_HPP
#define DELIMITER_MOVE_ACTION_HPP

#include "Action.hpp"
#include "ActionOptions/EndBehavior.hpp"

class DelimiterMoveAction: public Action {
private:
    ScreenSize m_size;
    std::string m_delimiters;
    Direction m_move_direction;
    EndBehavior m_end_behavior;

public:
    DelimiterMoveAction(
        ScreenSize size,
        std::string delimiters,
        Direction move_direction,
        EndBehavior end_behavior
    );
    DelimiterMoveAction(const DelimiterMoveAction&) = default;
    ~DelimiterMoveAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //DELIMITER_MOVE_ACTION_HPP
