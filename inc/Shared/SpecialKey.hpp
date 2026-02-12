///
/// @file: SpecialKey.hpp
/// @description: defines constants for special characters, i.e. backspace, esc,...
///
/// @date: 2026-01-25
/// @author: Moritz Pirer
///

#ifndef SPECIAL_KEY_HPP
#define SPECIAL_KEY_HPP

// MODO: find a nicer way to do this

enum class SpecialKey {
   ESCAPE,
   BACKSPACE,
   ENTER,
   ARROW_LEFT,
   ARROW_RIGHT,
   ARROW_UP,
   ARROW_DOWN,
   TAB,
   SHIFT_TAB
};

#endif //SPECIAL_KEY_HPP
