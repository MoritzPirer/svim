///
/// @file: DeleteAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef DELETE_ACTION_HPP
#define DELETE_ACTION_HPP

#include "RangedAction.hpp"

class DeleteAction: public RangedAction {
private:

public:
    DeleteAction(std::optional<Position> start, std::optional<Position> end):
        RangedAction(start, end) {}

    DeleteAction(const DeleteAction&) = default;
    ~DeleteAction() = default;

    virtual void applyTo(EditorState& state, ScreenSize size) override;
};

#endif //DELETE_ACTION_HPP
