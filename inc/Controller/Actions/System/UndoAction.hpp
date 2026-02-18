///
/// @file: UndoAction.hpp
/// @description: undoes the most recent undoable action
///
/// @date: 2026-02-16
/// @author: Moritz Pirer
///

#ifndef UNDO_ACTION_HPP
#define UNDO_ACTION_HPP

#include "../Action.hpp"
#include "../../../Shared/Types/Direction.hpp"

class UndoAction: public Action{
public:
    UndoAction() = default;
    UndoAction(const UndoAction&) = default;
    ~UndoAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //UNDO_ACTION_HPP
