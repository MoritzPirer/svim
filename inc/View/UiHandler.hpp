///
/// @file: UiHandler.hpp
/// @description: Handle the interaction with the user
///
/// @date: 2026-01-28
/// @author: Moritz Pirer
///

#ifndef UI_HANDLER_HPP
#define UI_HANDLER_HPP

#include "NcursesSession.hpp"

#include "../Shared/ScreenSize.hpp"
#include "../Shared/RenderInfo.hpp"

class UiHandler {
private:
    NcursesSession m_ncurses_session;

    /// @brief draws a line of text to the screen
    /// @param start_visual_row what row to start drawing. Gets updated to reflect the new
    ///     starting position after rendering this paragraph
    /// @param line the line to render
    void renderLine(int start_visual_row, const std::string& line);

    void writeString(const std::string& content);

    void setStyle(TextRole role);

    void renderTextArea(const RenderInfo& render_info);
    void renderCursor(const RenderInfo& render_info);
    void renderPanel(const RenderInfo& render_info);


    /// @brief translates special keys (e.g. escape, backspace) to the internal
    ///     standard defined in Shared/SpecialInputs.hpp. Normal keys are unaffected
    int translateInput(int original_input);
public:
    UiHandler() = default;
    UiHandler(const UiHandler&) = delete;
    ~UiHandler() = default;

    ScreenSize screenSize() const;
    void render(const RenderInfo& render_info);

    /// @brief waits for the user to input a key and then returns it
    int getInput();

};

#endif //UI_HANDLER_HPP
