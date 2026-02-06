#include "../../../inc/Controller/Control/EditorController.hpp"
#include "../../../inc/Controller/Control/FileHandler.hpp"
#include "../../../inc/Shared/StringHelpers.hpp"
#include "../../../inc/Controller/Control/Renderer.hpp"

using std::vector, std::string;

EditorController::EditorController(std::optional<string> file_path):
    m_mode_manager{ModeType::TOOL_MODE},
    m_settings{} {

    TextFile file;
    if (!file_path.has_value() || file_path->empty()) {
        std::filesystem::path name = FileHandler::getDefaultName(); 
        file = FileHandler::createFile(name);
    }
    else {
        file = FileHandler::openFile(file_path.value());      
    }
    m_state = EditorState(file);
}

RenderInfo EditorController::calculateRenderInfo(ScreenSize actual_size) {
    Renderer renderer(m_state, m_settings, m_mode_manager);

    vector<vector<RenderChunk>> metadata_rows = renderer.calculateMetadataRows(actual_size);

    ScreenSize text_area_size = actual_size;
    text_area_size.height -= metadata_rows.size();
    text_area_size.width -= renderer.calculateLineNumberWidth();

    return {
        renderer.calculateVisibleRows(text_area_size),
        metadata_rows,
        renderer.calculateLineNumbers(text_area_size),
        renderer.calculateLineNumberWidth(),
        renderer.calculateScreenPositionOfCursor(text_area_size),
        m_settings.isEnabled("render_color")
    };
}

ScreenSize EditorController::calculateTextAreaSize(const RenderInfo& render_info, const ScreenSize& total_size) {
    return {
        total_size.height - render_info.getPanelRowCount(),
        total_size.width - render_info.getAsideWidth()
    };
}

void EditorController::mainLoop() {
    while (m_state.getIsQuit() == false) {
        ScreenSize total_size = m_ui_handler.screenSize();
        RenderInfo render_info = calculateRenderInfo(total_size);
        m_ui_handler.render(render_info);
        int input = m_ui_handler.getInput();

        m_state.clearTemporaryMessages();
        vector<std::shared_ptr<Action>> actions = m_mode_manager.convertToAction(
            input,
            calculateTextAreaSize(render_info, total_size),
            m_settings
        );

        for (std::shared_ptr action : actions) {
            action->applyTo(m_state);
        } 
    }
}