///
/// @file: ChunkwiseMoveAction.hpp
/// @description: Move the character a by a chunk of text at a time
///
/// @date: 2026-01-30
/// @author: Moritz Pirer
///

#ifndef CHUNKWISE_MOVE_ACTION_HPP
#define CHUNKWISE_MOVE_ACTION_HPP

#include "Action.hpp"
#include "../../Shared/Destination.hpp"
#include "../../Shared/Scope.hpp"

class ChunkwiseMoveAction: public virtual Action {
private:
    void fileScopeMove(EditorState& state);
    void paragraphScopeMove(EditorState& state);

protected:
    const Scope m_scope;
    const Destination m_destination;

public:
    ChunkwiseMoveAction(Scope scope, Destination destination);
    ChunkwiseMoveAction(const ChunkwiseMoveAction&) = default;
    virtual ~ChunkwiseMoveAction() = default;

    virtual void applyTo(EditorState& state) override;
};

#endif //CHUNKWISE_MOVE_ACTION_HPP
