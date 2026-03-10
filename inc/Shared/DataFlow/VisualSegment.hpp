///
/// @file: VisualSegment.hpp
/// @description: A piece of text with a specific role in the rendered space
///
/// @date: 2026-02-02
/// @author: Moritz Pirer
///

#ifndef VISUAL_SEGMENT_HPP
#define VISUAL_SEGMENT_HPP

#include <string>

#include "TextRole.hpp"
#include "TextStyle.hpp"

struct VisualSegment {
    std::string content;
    TextRole role;
    TextStyle style;
};

#endif //VISUAL_SEGMENT_HPP
