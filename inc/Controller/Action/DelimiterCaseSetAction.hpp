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

    int getStartColumn(const EditorState& state, int row);
    int getEndColumn(const EditorState& state, int row, Position stop_position);

public:
    DelimiterCaseSetAction(
        std::string delimiters,
        std::string anti_delimiters,
        Direction direction,
        bool paragraph_is_delimiter,
        Case target_case
    );
    DelimiterCaseSetAction(const DelimiterCaseSetAction&) = default;
    ~DelimiterCaseSetAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //DELIMITER_CASE_SET_ACTION_HPP
