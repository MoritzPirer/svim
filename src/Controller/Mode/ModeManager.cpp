#include "../../../inc/Controller/Mode/ModeManager.hpp"
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Mode/TypingMode.hpp"

ModeManager::ModeManager(ModeType initial_mode) {
    changeMode(initial_mode);
}

void ModeManager::changeMode(ModeType new_mode) {
    switch (new_mode) {
        case ModeType::TOOL_MODE:
            m_current_mode = std::make_unique<ToolMode>();
            break;
        case ModeType::TYPING_MODE:
            m_current_mode = std::make_unique<TypingMode>();
            break;
        
        default:
            break;
    }
}

std::vector<std::shared_ptr<Action>> ModeManager::convertToAction(
    int input, ScreenSize size, Settings settings) {
    auto [new_mode, actions] = m_current_mode->parseInput(input, size, settings);
    changeMode(new_mode);

    return actions;
}

std::string ModeManager::getModeLabel() const {
    return "--" + m_current_mode->getModeLabel() + "--";
}