///
/// @file: CaseSetAction.hpp
/// @description: Sets a Span of characters to a given case
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef CASE_SET_ACTION_HPP
#define CASE_SET_ACTION_HPP

#include "../SpanAction.hpp"
#include "../Options/Case.hpp"

class CaseSetAction: public SpanAction {
private:
    Case m_target_case;
    std::vector<std::string> m_original_content;

    std::vector<std::string> getContentAsCase();
public:
    CaseSetAction(
        Position start,
        Position end,
        Case target_case
    );
    CaseSetAction(const CaseSetAction&) = default;
    ~CaseSetAction() = default;

    void apply(ExecutionContext& context) override;

    void undo(EditorState& state) override;

    bool canBeUndone() const override;
};

#endif //CASE_SET_ACTION_HPP
