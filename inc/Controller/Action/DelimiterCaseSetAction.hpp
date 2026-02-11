///
/// @file: DelimiterCaseSetAction.hpp
/// @description: moves backwards or forwards until a set of delimiters is reached or passed
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef DELIMITER_CASE_SET_ACTION_HPP
#define DELIMITER_CASE_SET_ACTION_HPP

#include "DelimiterAction.hpp"
#include "ActionOptions/Case.hpp"

class DelimiterCaseSetAction: public DelimiterAction {
private:
    Case m_target_case;

    void setCaseAt(EditorState& state, Position position);
public:
    DelimiterCaseSetAction(
        ScreenSize size,
        std::string delimiters,
        Direction direction,
        Case target_case
    );
    DelimiterCaseSetAction(const DelimiterCaseSetAction&) = default;
    ~DelimiterCaseSetAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //DELIMITER_CASE_SET_ACTION_HPP
