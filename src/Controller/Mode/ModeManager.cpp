#include "../../../inc/Controller/Mode/ModeManager.hpp"
#include "../../../inc/Controller/Mode/ToolMode.hpp"
#include "../../../inc/Controller/Mode/TypeMode.hpp"

ModeManager::ModeManager(ModeType initial_mode) {
    changeState(initial_mode);
}

void ModeManager::changeState(ModeType new_mode) {
    switch (new_mode) {
    case ModeType::TOOL_MODE:
        m_current_mode = std::make_unique<ToolMode>();
        break;
    case ModeType::TYPE_MODE:
        m_current_mode = std::make_unique<TypeMode>();
        break;
    
    default:
        break;
    }
}

std::shared_ptr<Action> ModeManager::convertToAction(int input) {
    auto [new_mode, action] = m_current_mode->parseInput(input);
    changeState(new_mode);

    return action;
}

std::string ModeManager::getModeLabel() const {
    return m_current_mode->getModeLabel();
}