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

class ModeManager {
private:
    std::unique_ptr<EditorMode> m_current_mode;
    
    void changeMode(ModeType new_mode);

public:
    ModeManager(ModeType initial_mode);
    ~ModeManager() = default;
    
    /// @brief generate an action based on the given input and the current mode.
    ///     May change the current mode
    /// @param input the user input
    /// @param size the current size of the text area
    /// @return a set of actions to execute
    std::vector<std::shared_ptr<Action>> convertToAction(
        int input, ScreenSize size, Settings settings);
    
    /// @return the name of the current mode
    std::string getModeLabel() const;
};

#endif //MODE_MANAGER_HPP
