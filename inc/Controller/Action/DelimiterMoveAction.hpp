///
/// @file: DelimiterMoveAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef DELIMITER_MOVE_ACTION_HPP
#define DELIMITER_MOVE_ACTION_HPP

#include "DelimiterAction.hpp"

class DelimiterMoveAction: public DelimiterAction {
public:
    DelimiterMoveAction(
        std::string delimiters,
        std::string anti_delimiters,
        Direction move_direction,
        EndBehavior end_behavior,
        bool paragraph_is_delimiter
    );
    DelimiterMoveAction(const DelimiterMoveAction&) = default;
    ~DelimiterMoveAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //DELIMITER_MOVE_ACTION_HPP
