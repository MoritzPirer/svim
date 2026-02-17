///
/// @file: ModeManager.hpp
/// @description: handles the Editor modes an the transitions between them
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef MODE_MANAGER_HPP
#define MODE_MANAGER_HPP

#include <memory>

#include "../Action/Action.hpp"
#include "EditorMode.hpp"
#include "../../Shared/Input.hpp"
#include "../Control/ParsingContext.hpp"
class ModeManager {
private:
    std::unique_ptr<EditorMode> m_current_mode;
    
    void changeMode(ModeType new_mode);

public:
    ModeManager(ModeType initial_mode);
    ~ModeManager() = default;
    
    std::optional<std::shared_ptr<Action>> convertToAction(Input input, ParsingContext context);
    
    /// @return the name of the current mode
    std::string getModeLabel() const;
};

#endif //MODE_MANAGER_HPP
