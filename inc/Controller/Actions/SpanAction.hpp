///
/// @file: SpanAction.hpp
/// @description: abstract base for an action that is performed between two positions
///
/// @date: 2026-02-11
/// @author: Moritz Pirer
///

#ifndef SPAN_ACTION_HPP
#define SPAN_ACTION_HPP

#include "Action.hpp"

class SpanAction: public Action {
private:
protected:
    Position m_start;
    Position m_end;
public:
    SpanAction(Position start, Position end);
    SpanAction(const SpanAction&) = default;
    ~SpanAction() = default;

    void apply(ExecutionContext& context) override = 0;
};


#endif //SPAN_ACTION_HPP
