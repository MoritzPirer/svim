#include <ncurses.h>

#include "../../inc/View/UiHandler.hpp"
#include "../../inc/Shared/SpecialInputs.hpp"

void UiHandler::renderLine(int start_visual_row, const std::string& line) {
    mvprintw(start_visual_row, 0, "%s", line.c_str());
}

void UiHandler::writeString(const std::string& content) {
    addstr(content.c_str());
}

void UiHandler::setStyle(TextRole role) {
    attrset(A_NORMAL);
    
    switch (role) {
        case TextRole::TEXT_NORMAL: {
            attron(COLOR_PAIR(1));
            break;
        }
        case TextRole::TEXT_HIGHLIGHT: {
            attron(COLOR_PAIR(2));
            break;
        }
        case TextRole::FILE_CHANGED: {
            attron(COLOR_PAIR(3));
            break;
        }
        case TextRole::FILE_NEW: {
            attron(COLOR_PAIR(4));
            break;
        }
        case TextRole::FILE_SAVED: {
            attron(COLOR_PAIR(5));
            break;
        }
        case TextRole::UI_ELEMENT: {
            attron(COLOR_PAIR(6));
            break;
        }
    }
}

void UiHandler::renderTextArea(const RenderInfo& render_info) {
    for (int i = 0; i < render_info.getTextAreaRowCount(); i++) {
        renderLine(i, render_info.getTextAreaRow(i));
    }
}

void UiHandler::renderCursor(const RenderInfo& render_info) {
    move(render_info.getCursorPosition().row, render_info.getCursorPosition().column);
    curs_set(1);
}

void UiHandler::renderPanel(const RenderInfo& render_info) {
    int metadata_offset = render_info.getTextAreaRowCount();
    move(metadata_offset, 0);

    for (int i = 0; i < render_info.getPanelRowCount(); i++) {
        for (auto& [content, role] : render_info.getPanelRow(i)) {
            if (render_info.shouldRenderColors()) {
                setStyle(role);
            }
            else {
                setStyle(TextRole::TEXT_NORMAL);
            }
            
            writeString(content);
        }  
        metadata_offset++;
        move(metadata_offset, 0);
    }
}

int UiHandler::translateInput(int original_input) {
    switch (original_input) {
        case '\n':
        case '\r':
        case KEY_ENTER:
            return INPUT_ENTER;
        
        case 27:
            return INPUT_ESCAPE;
        
        case KEY_BACKSPACE:
        case 127:
        case '\b':
            return INPUT_BACKSPACE;

        case KEY_LEFT:
            return ARROW_LEFT;
        case KEY_RIGHT:
            return ARROW_RIGHT;
        case KEY_UP:
            return ARROW_UP;
        case KEY_DOWN:
            return ARROW_DOWN;
        default:
            return original_input;
    }
}

ScreenSize UiHandler::screenSize() const {
    return {getmaxy(stdscr), getmaxx(stdscr)};
}

void UiHandler::render(const RenderInfo& render_info) {
    clear();

    renderTextArea(render_info);
    renderPanel(render_info);
    renderCursor(render_info);

    refresh();
}

int UiHandler::getInput() {
    return translateInput(getch());
}