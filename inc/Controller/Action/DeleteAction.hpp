///
/// @file: DeleteAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef DELETE_ACTION_HPP
#define DELETE_ACTION_HPP

#include <vector>

#include "Action.hpp"

class DeleteAction: public Action {
private:
    std::vector<std::string> m_deleted_content;
    Position m_start;
    Position m_end;
public:
    DeleteAction(Position start, Position end);
    DeleteAction(const DeleteAction&) = default;
    ~DeleteAction() = default;

    void apply(ExecutionContext& context) override;

    void undo(EditorState& state) override;

    bool canBeUndone() const override;
};

#endif //DELETE_ACTION_HPP
