#include <ncurses.h>

#include "../../inc/View/UiHandler.hpp"

void UiHandler::renderText(const RenderInfo& render_info) {
    for (int i = 0; i < render_info.getTextRowCount(); i++) {
        m_display_handler.renderLine(i, render_info.getTextRow(i));
    }
}

void UiHandler::renderCursor(const RenderInfo& render_info) {
    m_display_handler.renderCursor(render_info.getCursorPosition());
}

void UiHandler::renderMetadata(const RenderInfo& render_info) {
    int metadata_offset = render_info.getTextRowCount();
    move(metadata_offset, 0);

    for (int i = 0; i < render_info.getMetadataRowCount(); i++) {
        for (auto& [content, role] : render_info.getMetadataRow(i)) {
            if (render_info.shouldRenderColors()) {
                m_display_handler.setStyle(role);
            }
            else {
                m_display_handler.setStyle(TextRole::TEXT_NORMAL);
            }
            
            m_display_handler.addContent(content);
        }  
        metadata_offset++;
        // addstr(std::to_string(render_info.getMetadataRowCount()).c_str());
        move(metadata_offset, 0);
    }
}

ScreenSize UiHandler::screenSize() const {
    return m_display_handler.screenSize();
}

void UiHandler::render(const RenderInfo& render_info) {
    clear();

    renderText(render_info);
    renderMetadata(render_info);
    renderCursor(render_info);

    refresh();
}

int UiHandler::getInput() {
    int raw_input = m_input_handler.getInput();
    return m_input_handler.translateInput(raw_input);
}