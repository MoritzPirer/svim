///
/// @file: ParagraphSplittingAction.hpp
/// @description: description
///
/// @date: 2026-01-25
/// @author: Moritz Pirer
///

#ifndef PARAGRAPH_SPLITTING_ACTION_HPP
#define PARAGRAPH_SPLITTING_ACTION_HPP

#include "Action.hpp"

class ParagraphSplittingAction: public Action {
public:
    ParagraphSplittingAction() = default;
    ParagraphSplittingAction(const ParagraphSplittingAction &) = default;
    ~ParagraphSplittingAction() = default;

    void applyTo(EditorState& state);
};

#endif //PARAGRAPH_SPLITTING_ACTION_HPP
