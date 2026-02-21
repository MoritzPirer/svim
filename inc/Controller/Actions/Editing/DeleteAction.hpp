///
/// @file: DeleteAction.hpp
/// @description: Deletes a given span of text
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef DELETE_ACTION_HPP
#define DELETE_ACTION_HPP

#include <vector>

#include "../Action.hpp"

class DeleteAction: public Action {
private:
    std::vector<std::string> m_deleted_content;
    Position m_start;
    Position m_end;
    Position m_cursor; //where to restore the cursor to

    /// @brief other is directly before or directly after this delete section
    bool isAdjacent(std::shared_ptr<DeleteAction> other) const;

    /// @brief m_deleted_content has at least one non-empty line
    bool hasContent() const;
    
    void absorbBeforeCurrent(std::shared_ptr<DeleteAction> other);
    void absorbAfterCurrent(std::shared_ptr<DeleteAction> other);
    
public:
    DeleteAction(Position start, Position end, Position cursor);
    DeleteAction(const DeleteAction&) = default;
    ~DeleteAction() = default;

    void apply(ExecutionContext& context) override;

    bool canBeUndone() const override;

    void undo(EditorState& state) override;

    bool canAbsorb(const std::shared_ptr<Action>& action) const override;

    void absorb(const std::shared_ptr<Action>& action) override;

};

#endif //DELETE_ACTION_HPP
