///
/// @file: InsertAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef INSERT_ACTION_HPP
#define INSERT_ACTION_HPP

#include <vector>

#include "Action.hpp"

class InsertAction: public Action {
private:
    std::vector<std::string> m_content;
    Position m_start;
public:
    InsertAction(std::vector<std::string> content, Position start);
    InsertAction(const InsertAction&) = default;
    ~InsertAction() = default;

    void apply(ExecutionContext& context) override;

    void undo(EditorState& state) override;

    bool canBeUndone() const override;
};

#endif //INSERT_ACTION_HPP
