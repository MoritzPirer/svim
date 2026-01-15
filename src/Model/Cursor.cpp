#include "../../inc/Model/Cursor.hpp"

Cursor::Cursor():
    m_row{0},
    m_actual_column{0},
    m_desired_column{0}
    {}

void Cursor::adjustActualColumn(size_t new_line_length) {
    if (new_line_length > m_desired_column) { // new line is long enough
        m_actual_column = m_desired_column;
    }
    else {
        m_actual_column = new_line_length == 0? 0 : new_line_length - 1;
    }
}

void Cursor::moveUp(size_t new_line_length) {
    m_row--;
    adjustActualColumn(new_line_length);
}

void Cursor::moveDown(size_t new_line_length) {
    m_row++;
    adjustActualColumn(new_line_length);
}

void Cursor::moveLeft() {
    m_actual_column--;
    m_desired_column = m_actual_column;
}

void Cursor::moveRight() {
    m_actual_column++;
    m_desired_column = m_actual_column;
}