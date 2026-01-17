#include "../../inc/Model/Cursor.hpp"

Cursor::Cursor():
    m_position{0, 0} {}

void Cursor::setRow(int row) {
    m_position.row = row;
}

void Cursor::setColumn(int column) {
    m_position.column = column;
}

void Cursor::moveUpLogical() {
    m_position.row--;
}

void Cursor::moveUpVisual(int screen_with) {
    m_position.column -= screen_with;
}

void Cursor::moveDownLogical() {
    m_position.row++;
}

void Cursor::moveDownVisual(int screen_with) {
    m_position.column += screen_with;
}

void Cursor::moveLeft() {
    m_position.column--;
}

void Cursor::moveRight() {
    m_position.column++;
}