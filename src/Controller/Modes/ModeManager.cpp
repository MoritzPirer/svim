#include "../../../inc/Controller/Modes/ModeManager.hpp"
#include "../../../inc/Controller/Modes/ToolMode.hpp"
#include "../../../inc/Controller/Modes/TypingMode.hpp"

ModeManager::ModeManager(ModeType initial_mode) {
    m_all_modes = {
        {ModeType::TOOL_MODE, std::make_shared<ToolMode>()},
        {ModeType::TYPING_MODE, std::make_shared<TypingMode>()},
    };

    changeMode(initial_mode);
}

void ModeManager::changeMode(ModeType new_mode) {
    m_current_mode = m_all_modes.at(new_mode);
}

std::optional<std::shared_ptr<Action>> ModeManager::convertToAction(
    Input input, ParsingContext context) {
    ParseResult result = m_current_mode->parseInput(input, context);
    if (result.mode.has_value()) {
        changeMode(*result.mode);
    }

    return result.action;
}

std::string ModeManager::getModeLabel() const {
    return "--" + m_current_mode->getModeLabel() + "--";
}