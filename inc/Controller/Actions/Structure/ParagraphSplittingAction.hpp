///
/// @file: ParagraphSplittingAction.hpp
/// @description: Splits a paragraph into two parts at a given position
///
/// @date: 2026-01-25
/// @author: Moritz Pirer
///

#ifndef PARAGRAPH_SPLITTING_ACTION_HPP
#define PARAGRAPH_SPLITTING_ACTION_HPP

#include "../Action.hpp"

class ParagraphSplittingAction: public Action {
private:
    Position m_first_after_split;
public:
    ParagraphSplittingAction(Position first_after_split);
    ParagraphSplittingAction(const ParagraphSplittingAction &) = default;
    ~ParagraphSplittingAction() = default;

    void apply(ExecutionContext& context) override;

    void undo(EditorState& state) override;

    bool canBeUndone() const override;;
};

#endif //PARAGRAPH_SPLITTING_ACTION_HPP
