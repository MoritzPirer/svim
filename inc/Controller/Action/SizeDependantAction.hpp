///
/// @file: SizeDependantAction.hpp
/// @description: Blueprint for actions that need to know about the current screen size
///
/// @date: 2026-01-26
/// @author: Moritz Pirer
///

#ifndef SIZE_DEPENDANT_ACTION_HPP
#define SIZE_DEPENDANT_ACTION_HPP

#include "Action.hpp"

class SizeDependantAction: public Action {
protected:
    ScreenSize m_size;
public:
    SizeDependantAction(ScreenSize size): m_size{size} {}
    SizeDependantAction(const SizeDependantAction&) = default;
    ~SizeDependantAction() = default;

    virtual void applyTo(EditorState& state) override = 0;
};

#endif //SIZE_DEPENDANT_ACTION_HPP
