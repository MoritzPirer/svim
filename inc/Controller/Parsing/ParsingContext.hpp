///
/// @file: ParsingContext.hpp
/// @description: Environmental information needed to Parse input into actions
///
/// @date: 2026-02-17
/// @author: Moritz Pirer
///

#ifndef PARSING_CONTEXT_HPP
#define PARSING_CONTEXT_HPP

#include "../../Shared/Types/ScreenSize.hpp"

class EditorState;
class Settings;

struct ParsingContext {
    EditorState& state;
    ScreenSize actual_size;
    ScreenSize text_area_size;
    Settings& settings;    
};

#endif //PARSING_CONTEXT_HPP
