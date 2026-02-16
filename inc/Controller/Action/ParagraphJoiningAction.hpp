///
/// @file: ParagraphJoiningAction.hpp
/// @description: description
///
/// @date: 2026-01-25
/// @author: Moritz Pirer
///

#ifndef PARAGRAPH_JOINING_ACTION_HPP
#define PARAGRAPH_JOINING_ACTION_HPP

#include "Action.hpp"

class ParagraphJoiningAction: public Action {
public:
    ParagraphJoiningAction() = default;
    ParagraphJoiningAction(const ParagraphJoiningAction &) = default;
    ~ParagraphJoiningAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //PARAGRAPH_JOINING_ACTION_HPP
