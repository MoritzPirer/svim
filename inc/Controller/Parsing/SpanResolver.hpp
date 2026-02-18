///
/// @file: SpanResolver.hpp
/// @description: Generate a Span consisting of inclusive start and end positions
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef SPAN_RESOLVER_HPP
#define SPAN_RESOLVER_HPP

#include "../../Shared/Types/Position.hpp"
#include "../../Model/EditorState.hpp"
#include "RangeSettings.hpp"
#include "ScopeSettings.hpp"

namespace SpanResolver {

    std::pair<Position, Position> fromDelimiter(EditorState& state, RangeSettings settings);

    std::pair<Position, Position> fromScope(EditorState& state, ScopeSettings settings);
};

#endif //SPAN_RESOLVER_HPP
