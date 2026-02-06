///
/// @file: RenderInfo.hpp
/// @description: the data that should be rendered
///
/// @date: 2026-01-29
/// @author: Moritz Pirer
///

#ifndef RENDER_INFO_HPP
#define RENDER_INFO_HPP

#include <vector>
#include <string>

#include "Position.hpp"
#include "../Controller/Control/RenderChunk.hpp"

class RenderInfo {
private:
    std::vector<std::vector<RenderChunk>> m_text_area_rows;

    std::vector<std::vector<RenderChunk>> m_panel_rows;
    std::vector<RenderChunk> m_aside_rows;
    int m_aside_width;
    Position m_cursor_position;
    
    bool m_render_colors;

public:
    RenderInfo(
        std::vector<std::vector<RenderChunk>> text_area_rows,
        std::vector<std::vector<RenderChunk>> panel_rows,
        std::vector<RenderChunk> aside_rows,
        int aside_width,
        Position cursor_position,
        bool render_in_color
    );

    RenderInfo(const RenderInfo&) = default;
    ~RenderInfo() = default;

    const std::vector<RenderChunk>& getTextAreaRow(int index) const;
    int getTextAreaRowCount() const;
    
    const std::vector<RenderChunk>& getPanelRow(int index) const;
    int getPanelRowCount() const;

    RenderChunk getAsideRow(int index) const;
    int getAsideRowCount() const;
    int getAsideWidth() const;

    bool shouldRenderColors() const;

    const Position& getCursorPosition() const;
};

#endif //RENDER_INFO_HPP
