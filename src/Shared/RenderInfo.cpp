#include "../../inc/Shared/RenderInfo.hpp"

RenderInfo::RenderInfo(
    std::vector<std::string> visual_rows,
    std::vector<std::vector<RenderChunk>> metadata_rows,
    Position cursor_position,
    bool render_colors
):
    m_visual_rows{visual_rows},
    m_metadata_rows{metadata_rows},
    m_cursor_position{cursor_position},
    m_render_colors{render_colors} {}

const std::string& RenderInfo::getTextRow(int index) const {
    return m_visual_rows.at(index);
}

int RenderInfo::getTextRowCount() const {
    return m_visual_rows.size();
}

const std::vector<RenderChunk>& RenderInfo::getMetadataRow(int index) const {
    return m_metadata_rows.at(index);
}

int RenderInfo::getMetadataRowCount() const {
    return m_metadata_rows.size();
}

bool RenderInfo::shouldRenderColors() const {
    return m_render_colors;
}
const Position& RenderInfo::getCursorPosition() const { return m_cursor_position; }