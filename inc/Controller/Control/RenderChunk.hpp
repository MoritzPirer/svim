///
/// @file: RenderChunk.hpp
/// @description: A piece of text with a specific role in the rendered space
///
/// @date: 2026-02-02
/// @author: Moritz Pirer
///

#ifndef RENDER_CHUNK_HPP
#define RENDER_CHUNK_HPP

#include <string>

#include "TextRole.hpp"

struct RenderChunk {
    std::string content;
    TextRole role;
};

#endif //RENDER_CHUNK_HPP
