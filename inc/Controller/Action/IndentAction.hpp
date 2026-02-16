///
/// @file: IndentAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef INDENT_ACTION_HPP
#define INDENT_ACTION_HPP

#include "Action.hpp"

class IndentAction: public Action {
private:
    int m_indent_width;
public:
    IndentAction(
        int indent_width
    );
    IndentAction(const IndentAction&) = default;
    ~IndentAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //INDENT_ACTION_HPP
