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

class Cursor {
private:
    size_t m_row;
    size_t m_actual_column;
    size_t m_desired_column;

    void adjustActualColumn(size_t new_line_length);

public:
    Cursor();
    Cursor(const Cursor&) = default;
    ~Cursor() = default;

    size_t getRow() const { return m_row; }
    size_t getColumn() const { return m_actual_column; }

    void moveUp(size_t new_line_length);
    void moveDown(size_t new_line_length);
    void moveLeft();
    void moveRight();
};

#endif //CURSOR_HPP
