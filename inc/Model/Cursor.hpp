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
    Position m_position;
    
public:
    Cursor();
    Cursor(const Cursor&) = default;
    ~Cursor() = default;

    Position getPosition() const { return m_position; }
    int getRow() const { return m_position.row; }
    int getColumn() const { return m_position.column; }

    void setRow(int row);
    void setColumn(int column);

    /// @brief moves to the end of the previous paragraph
    ///     or the start of the current if already at the top
    void moveUpLogical();
    
    /// @brief moves to the same column in the previous visual line, or to the
    ///     end of the row if it's shorter
    /// @param screen_width the current width of the writing area 
    void moveUpVisual(int screen_width);

    /// @brief move to the first column of the next paragraph or the end of the 
    ///     current one if already in the last one
    void moveDownLogical();

    /// @brief  moves to the same column in the next visual line, or the end of
    ///     the new line if it's shorter
    /// @param screen_width 
    void moveDownVisual(int screen_width);

    /// @brief move one character to the left or to the end of the
    ///     previous paragraph if already at the start
    void moveLeft();

    /// @brief move one character to the right or to the start
    ///     of the next paragraph if already at the end
    void moveRight();
};

#endif //CURSOR_HPP
