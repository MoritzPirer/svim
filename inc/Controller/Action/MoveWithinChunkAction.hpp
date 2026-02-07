///
/// @file: MoveWithinChunkAction.hpp
/// @description: Move the character a by a chunk of text at a time
///
/// @date: 2026-01-30
/// @author: Moritz Pirer
///

#ifndef MOVE_WITHIN_CHUNK_ACTION_HPP
#define MOVE_WITHIN_CHUNK_ACTION_HPP

#include "Action.hpp"
#include "../../Shared/Destination.hpp"
#include "../../Shared/Scope.hpp"

class MoveWithinChunkAction: public virtual Action {
private:
    void fileScopeMove(EditorState& state);
    void paragraphScopeMove(EditorState& state);
    void lineScopeMove(EditorState& state);
    void delimiterMove(EditorState& state);

protected:
    std::optional<Scope> m_scope;
    std::optional<std::string> m_delimiters;
    const Destination m_destination;
    const ScreenSize m_size;

public:
    MoveWithinChunkAction(Scope scope, Destination destination, ScreenSize size);
    MoveWithinChunkAction(std::string delimiters, Destination destination, ScreenSize size);
    MoveWithinChunkAction(const MoveWithinChunkAction&) = default;
    virtual ~MoveWithinChunkAction() = default;

    virtual void applyTo(EditorState& state) override;
};

#endif //MOVE_WITHIN_CHUNK_ACTION_HPP
