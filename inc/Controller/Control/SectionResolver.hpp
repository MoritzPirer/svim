///
/// @file: RangeResolver.hpp
/// @description: description
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef SECTION_RESOLVER_HPP
#define SECTION_RESOLVER_HPP

#include "../../Shared/Position.hpp"
#include "../../Model/EditorState.hpp"
#include "RangeSettings.hpp"
#include "ScopeSettings.hpp"

namespace SectionResolver {

    std::pair<Position, Position> fromDelimiter(EditorState& state, RangeSettings settings);

    std::pair<Position, Position> fromScope(EditorState& state, ScopeSettings settings);
};

#endif //SECTION_RESOLVER_HPP
