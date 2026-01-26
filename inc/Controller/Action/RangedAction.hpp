///
/// @file: RangedAction.hpp
/// @description: description
///
/// @date: 2026-01-25
/// @author: Moritz Pirer
///

#ifndef RANGED_ACTION_HPP
#define RANGED_ACTION_HPP

#include <optional>

#include "Action.hpp"

class RangedAction: public Action {
protected:
    std::optional<Position> m_start;
    std::optional<Position> m_end;
public:
    RangedAction(std::optional<Position> start, std::optional<Position> end):
        m_start{start},
        m_end{end} {}
        
    RangedAction(const RangedAction&) = default;
    virtual ~RangedAction() = default;

    virtual void applyTo(EditorState& state) override = 0;
};

#endif //RANGED_ACTION_HPP
