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
#include <unordered_map>

#include "../Actions/Action.hpp"
#include "EditorMode.hpp"
#include "../../Shared/Types/Input.hpp"
#include "../Parsing/ParsingContext.hpp"

class ModeManager {
private:
    std::shared_ptr<EditorMode> m_current_mode;
    std::unordered_map<ModeType, std::shared_ptr<EditorMode>> m_all_modes;
    
    void changeMode(ModeType new_mode);

public:
    ModeManager(ModeType initial_mode);
    ~ModeManager() = default;
    
    std::optional<std::shared_ptr<Action>> convertToAction(Input input, ParsingContext context);
    
    /// @return the name of the current mode
    std::string getModeLabel() const;
};

#endif //MODE_MANAGER_HPP
