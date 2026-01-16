///
/// @file: Cursor.hpp
/// @description: description
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <stddef.h>

#include "../../inc/Model/TextFile.hpp"
#include "../../inc/Controller/Position.hpp"

class Cursor {
private:
    Position m_actual_position;
    Position m_desired_position;
    void adjustActualColumn(size_t new_line_length);

public:
    Cursor();
    Cursor(const Cursor&) = default;
    ~Cursor() = default;

    Position getPosition() const { return m_actual_position; }

    void moveUp(size_t new_line_length);
    void moveDown(size_t new_line_length);
    void moveLeft();
    void moveRight();
};

#endif //CURSOR_HPP
