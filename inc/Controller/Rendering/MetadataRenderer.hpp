///
/// @file: MetadataRenderer.hpp
/// @description: description
///
/// @date: 2026-03-16
/// @author: Moritz Pirer
///

#ifndef METADATA_RENDERER_HPP
#define METADATA_RENDERER_HPP

#include "../../Shared/DataFlow/VisualSegment.hpp"
#include "../../Model/EditorState.hpp"
#include "../Modes/ModeManager.hpp"
#include "../Services/Settings/Settings.hpp"

class MetadataRenderer {
private:
    const EditorState& c_state;
    const Settings& c_settings;
    const ModeManager& c_mode_manager;

    std::vector<VisualSegment> getSeperatorChunks(ScreenSize actual_size);
    std::vector<VisualSegment> getCharacterCountChunks();
    std::vector<VisualSegment> getWordCountChunks();
    std::vector<VisualSegment> getParagraphCountChunks();
    std::vector<VisualSegment> getCursorPositionChunks();
    std::vector<VisualSegment> getEditorModeChunks();
    std::vector<VisualSegment> getFileNameChunks();
    std::vector<VisualSegment> getSaveIconChunks();
    std::vector<VisualSegment> getVersionChunks();

    /// @brief organize the metadata chunks into rows
    /// @return a vector where each element is a set of Chunks to be rendered in that row
    std::vector<std::vector<VisualSegment>> reorganizeMetadataRows(
        std::vector<VisualSegment> ordered_chunks, ScreenSize actual_size);

public:
    MetadataRenderer(const EditorState& state, const Settings& settings, const ModeManager& mode_manager);
    MetadataRenderer(const MetadataRenderer&) = default;
    ~MetadataRenderer() = default;

    /// @brief calculates the rows that should be shown for the metadata panel
    /// @param actual_size the total size of the screen (not only the text area)
    /// @return a vector where each element is a set of VisualSegments
    /// that should be rendered in the same row of the screen 
    std::vector<std::vector<VisualSegment>> calculateMetadataRows(ScreenSize actual_size);

    std::vector<VisualSegment> calculateTemporaryRows(ScreenSize actual_size);
};

#endif //METADATA_RENDERER_HPP
