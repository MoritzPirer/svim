///
/// @file: ModeManager.hpp
/// @description: description
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
    
    void changeState(ModeType new_mode);
public:
    ModeManager(ModeType initial_mode);
    ~ModeManager() = default;
    
    std::shared_ptr<Action> convertToAction(int input, ScreenSize size);
    std::string getModeLabel() const;
};

#endif //MODE_MANAGER_HPP
