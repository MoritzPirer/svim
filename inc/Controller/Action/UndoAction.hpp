///
/// @file: UndoAction.hpp
/// @description: describes an action that moves the cursor on character or visual row
///
/// @date: 2026-02-16
/// @author: Moritz Pirer
///

#ifndef UNDO_ACTION_HPP
#define UNDO_ACTION_HPP

#include "Action.hpp"
#include "../../Shared/Direction.hpp"

class UndoAction: public Action{
private:
public:
    UndoAction() = default;
    UndoAction(const UndoAction&) = default;
    ~UndoAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //UNDO_ACTION_HPP
