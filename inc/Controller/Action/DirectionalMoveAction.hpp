///
/// @file: DirectionalMoveAction.hpp
/// @description: Move the character by a chunk of text at a time in a specific direction
///
/// @date: 2026-01-31
/// @author: Moritz Pirer
///

#ifndef DIRECTIONAL_MOVE_ACTION_HPP
#define DIRECTIONAL_MOVE_ACTION_HPP

#include "ChunkwiseMoveAction.hpp"
#include "SizeDependantAction.hpp"
#include "../../Shared/Direction.hpp"

class DirectionalMoveAction: public ChunkwiseMoveAction, public SizeDependantAction {
private:
    Direction m_direction;

    std::string getDelimiters() const;
    
    /// @brief moves in the specified direction until the next move would place it on a delimiter
    ///     or until it cannot move further. If the starting position is right next to a delimiter
    ///     the the delimiter and any delimiters immediatly after it are skipped
    void moveUntilDelimiter(EditorState& state) const;
    
    void moveOverDelimiter(EditorState& state) const;
public:
    DirectionalMoveAction(Scope scope, Destination destination, ScreenSize size, Direction direction);
    DirectionalMoveAction(const DirectionalMoveAction&) = default;
    ~DirectionalMoveAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //DIRECTIONAL_MOVE_ACTION_HPP
