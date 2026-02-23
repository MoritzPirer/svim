///
/// @file: ExecutionContext.hpp
/// @description: Provides the environment actions are performed in
///
/// @date: 2026-02-16
/// @author: Moritz Pirer
///

#ifndef EXECUTION_CONTEXT_HPP
#define EXECUTION_CONTEXT_HPP

class EditorState;
class UndoRedoManager;

struct ExecutionContext {
    EditorState& state;
    UndoRedoManager& undoRedoManager;
};

#endif //EXECUTION_CONTEXT_HPP
