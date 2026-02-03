#include "../../inc/Shared/RenderInfo.hpp"

RenderInfo::RenderInfo(
    std::vector<std::string> text_area_rows,
    std::vector<std::vector<RenderChunk>> panel_rows,
    Position cursor_position,
    bool render_colors
):
    m_text_area_rows{text_area_rows},
    m_panel_rows{panel_rows},
    m_cursor_position{cursor_position},
    m_render_colors{render_colors} {}

const std::string& RenderInfo::getTextAreaRow(int index) const {
    return m_text_area_rows.at(index);
}

int RenderInfo::getTextAreaRowCount() const {
    return m_text_area_rows.size();
}

const std::vector<RenderChunk>& RenderInfo::getPanelRow(int index) const {
    return m_panel_rows.at(index);
}

int RenderInfo::getPanelRowCount() const {
    return m_panel_rows.size();
}

bool RenderInfo::shouldRenderColors() const {
    return m_render_colors;
}
const Position& RenderInfo::getCursorPosition() const { return m_cursor_position; }