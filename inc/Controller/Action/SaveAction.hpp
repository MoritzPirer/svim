///
/// @file: SaveAction.hpp
/// @description: saves the current state of the file to the location specified by it
///
/// @date: 2026-01-26
/// @author: Moritz Pirer
///

#ifndef SAVE_ACTION_HPP
#define SAVE_ACTION_HPP

#include "Action.hpp"
#include "ActionOptions/SaveConfirmation.hpp"

class SaveAction: public Action {
    SaveConfirmation m_confirm_save;
public:
    SaveAction(SaveConfirmation confirm_save);
    SaveAction(const SaveAction&) = default;
    ~SaveAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //SAVE_ACTION_HPP
