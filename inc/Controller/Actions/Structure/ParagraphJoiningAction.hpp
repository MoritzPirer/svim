///
/// @file: ParagraphJoiningAction.hpp
/// @description: joins two paragraphs into one
///
/// @date: 2026-01-25
/// @author: Moritz Pirer
///

#ifndef PARAGRAPH_JOINING_ACTION_HPP
#define PARAGRAPH_JOINING_ACTION_HPP

#include "../Action.hpp"

class ParagraphJoiningAction: public Action {
private:
    Position m_joining_position;
    std::optional<Position> m_first_of_joinee;

public:
    ParagraphJoiningAction(Position joining_position);
    ParagraphJoiningAction(const ParagraphJoiningAction &) = default;
    ~ParagraphJoiningAction() = default;

    void apply(ExecutionContext& context) override;

    void undo(EditorState& state) override;

    bool canBeUndone() const override;
};

#endif //PARAGRAPH_JOINING_ACTION_HPP
