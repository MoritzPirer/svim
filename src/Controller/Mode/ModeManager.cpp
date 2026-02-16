#include "../../../inc/Controller/Mode/ModeManager.hpp"
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Mode/TypingMode.hpp"

ModeManager::ModeManager(ModeType initial_mode) {
    changeMode(initial_mode);
}

void ModeManager::changeMode(ModeType new_mode) {
    switch (new_mode) {
    case ModeType::TOOL_MODE: {
        m_current_mode = std::make_unique<ToolMode>();
        break;
    }
    
    case ModeType::TYPING_MODE: {
        m_current_mode = std::make_unique<TypingMode>();
        break;
    }
    
    default: {
        break;
    }

    }
}

std::vector<std::shared_ptr<Action>> ModeManager::convertToAction(
    Input input, ScreenSize actual_size, ScreenSize text_area_size, Settings settings, const EditorState& state) {
    ParseResult result = m_current_mode->parseInput(input, actual_size, text_area_size, settings, state);
    if (result.mode.has_value()) {
        changeMode(*result.mode);
    }

    return result.actions;
}

std::string ModeManager::getModeLabel() const {
    return "--" + m_current_mode->getModeLabel() + "--";
}