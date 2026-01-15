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


    void render();

public:
    Display() = default;
    ~Display() = default;

    void mainLoop();
};

#endif //DISPLAY_HPP
