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