///
/// @file: Display.hpp
/// @description: description
///
/// @date: 2026-01-14
/// @author: Moritz Pirer
///

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "NcursesSession.hpp"
#include "../Controller/EditorController.hpp"

class Display {
private:
    NcursesSession m_ncurses_session;
    EditorController m_controller;

    int screenHeight();
    int screenWidth();

    int visualRowsNeeded(size_t line_length);
    
    void renderCursor(size_t visual_row_of_cursor);

    void render();

public:
    Display() = default;
    ~Display() = default;

    void mainLoop();
};

#endif //DISPLAY_HPP
