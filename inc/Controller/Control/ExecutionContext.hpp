///
/// @file: ExecutionContext.hpp
/// @description: description
///
/// @date: 2026-02-16
/// @author: Moritz Pirer
///

#ifndef EXECUTION_CONTEXT_HPP
#define EXECUTION_CONTEXT_HPP

class EditorState;
class History;

struct ExecutionContext {
    EditorState& state;
    History& history;
};

#endif //EXECUTION_CONTEXT_HPP
