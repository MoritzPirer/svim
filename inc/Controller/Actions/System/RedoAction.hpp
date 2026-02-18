///
/// @file: RedoAction.hpp
/// @description: re-does the most recenty undone action
///
/// @date: 2026-02-16
/// @author: Moritz Pirer
///

#ifndef REDO_ACTION_HPP
#define REDO_ACTION_HPP

#include "../Action.hpp"
#include "../../../Shared/Types/Direction.hpp"

class RedoAction: public Action{
public:
    RedoAction() = default;
    RedoAction(const RedoAction&) = default;
    ~RedoAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //REDO_ACTION_HPP
