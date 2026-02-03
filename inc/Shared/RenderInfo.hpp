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
    std::vector<std::string> m_visual_rows;

    std::vector<std::vector<RenderChunk>> m_metadata_rows;
    std::string m_mode_label;
    Position m_cursor_position;
    
    bool m_render_colors;

public:
    RenderInfo(
        std::vector<std::string> visual_rows,
        std::vector<std::vector<RenderChunk>> metadata_rows,
        Position cursor_position,
        bool render_in_color
    );

    RenderInfo(const RenderInfo&) = default;
    ~RenderInfo() = default;

    const std::string& getTextRow(int index) const;
    int getTextRowCount() const;
    
    const std::vector<RenderChunk>& getMetadataRow(int index) const;
    int getMetadataRowCount() const;

    bool shouldRenderColors() const;

    const Position& getCursorPosition() const;
};

#endif //RENDER_INFO_HPP
