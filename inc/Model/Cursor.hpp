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

    void moveUpLogical();
    void moveUpVisual(int screen_width);
    void moveDownLogical();
    void moveDownVisual(int screen_width);
    void moveLeft();
    void moveRight();
};

#endif //CURSOR_HPP
