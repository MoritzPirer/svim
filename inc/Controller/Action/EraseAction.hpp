///
/// @file: EraseAction.hpp
/// @description: description
///
/// @date: 2026-01-30
/// @author: Moritz Pirer
///

#ifndef ERASE_ACTION_HPP
#define ERASE_ACTION_HPP

#include "Action.hpp"

class EraseAction: public Action {
private:
    int m_offset;
    bool m_allow_overhang_erase;

    std::optional<Position> findErasePosition(const EditorState& state);
public:
    EraseAction(int offset, bool allow_overhang_erase = true);
    EraseAction(const EraseAction&) = default;
    ~EraseAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //ERASE_ACTION_HPP
