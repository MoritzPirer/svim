///
/// @file: UnindentAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef UNINDENT_ACTION_HPP
#define UNINDENT_ACTION_HPP

#include "Action.hpp"

class UnindentAction: public Action {
private:
    int m_indent_width;

public:
    UnindentAction(
        int indent_width
    );
    UnindentAction(const UnindentAction&) = default;
    ~UnindentAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //UNINDENT_ACTION_HPP
