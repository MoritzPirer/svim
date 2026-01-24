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
#include "../Controller/Control/EditorController.hpp"

class Display {
private:
    NcursesSession m_ncurses_session;
    EditorController m_controller;

    int screenHeight();
    int screenWidth();

    ScreenSize screenSize();
    
    void renderEmptyLine(int& visual_row);
    void renderLine(int& visual_row, const std::string& line_to_render);
    void renderText();
    void renderCursor();
    void renderMetadata();

    void render();

public:
    Display(const std::string& file_path);
    Display();
    ~Display() = default;

    void mainLoop();
};

#endif //DISPLAY_HPP
