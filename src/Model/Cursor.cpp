#include "../../inc/Model/Cursor.hpp"

Cursor::Cursor():
    m_actual_position{0, 0},
    m_desired_position{0, 0}
    {}

void Cursor::adjustActualColumn(size_t new_line_length) {
    if (new_line_length > m_desired_position.column) { // new line is long enough
        m_actual_position.column = m_desired_position.column;
    }
    else {
        m_actual_position.column = new_line_length == 0? 0 : new_line_length - 1;
    }
}

void Cursor::moveUp(size_t new_line_length) {
    m_actual_position.row--;
    m_desired_position.row--;
    adjustActualColumn(new_line_length);
}

void Cursor::moveDown(size_t new_line_length) {
    m_actual_position.row++;
    m_desired_position.row++;
    adjustActualColumn(new_line_length);
}

void Cursor::moveLeft() {
    m_actual_position.column--;
    m_desired_position.column = m_actual_position.column;
}

void Cursor::moveRight() {
    m_actual_position.column++;
    m_desired_position.column = m_actual_position.column;
}