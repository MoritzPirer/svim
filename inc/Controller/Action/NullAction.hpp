///
/// @file: NullAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef NULL_ACTION_HPP
#define NULL_ACTION_HPP

#include "Action.hpp"

class NullAction: public Action {
public:
    NullAction() = default;
    NullAction(const NullAction&) = default;
    ~NullAction() = default;

    void applyTo(EditorState& state) override {
        (void) state;
    }
};

#endif //NULL_ACTION_HPP
