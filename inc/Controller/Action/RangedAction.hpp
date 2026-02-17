///
/// @file: RangedAction.hpp
/// @description: abstract base for an action that is performed from the cursor position to the first occurrence of a delimiter
///
/// @date: 2026-02-11
/// @author: Moritz Pirer
///

#ifndef RANGED_ACTION_HPP
#define RANGED_ACTION_HPP

#include "Action.hpp"
#include "../Control/RangeSettings.hpp"

class RangedAction: public Action {
private:
protected:
    Position m_start;
    Position m_end;
public:
    RangedAction(Position start, Position end);
    RangedAction(const RangedAction&) = default;
    ~RangedAction() = default;

    void apply(ExecutionContext& context) override = 0;
};


#endif //RANGED_ACTION_HPP
