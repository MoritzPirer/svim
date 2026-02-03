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
    std::vector<std::string> m_text_area_rows;

    std::vector<std::vector<RenderChunk>> m_panel_rows;
    Position m_cursor_position;
    
    bool m_render_colors;

public:
    RenderInfo(
        std::vector<std::string> text_area_rows,
        std::vector<std::vector<RenderChunk>> panel_rows,
        Position cursor_position,
        bool render_in_color
    );

    RenderInfo(const RenderInfo&) = default;
    ~RenderInfo() = default;

    const std::string& getTextAreaRow(int index) const;
    int getTextAreaRowCount() const;
    
    const std::vector<RenderChunk>& getPanelRow(int index) const;
    int getPanelRowCount() const;

    bool shouldRenderColors() const;

    const Position& getCursorPosition() const;
};

#endif //RENDER_INFO_HPP
