///
/// @file: QuitAction.hpp
/// @description: quits the editor
///
/// @date: 2026-01-26
/// @author: Moritz Pirer
///

#ifndef QUIT_ACTION_HPP
#define QUIT_ACTION_HPP

#include "Action.hpp"
#include "ActionOptions/QuitMode.hpp"

class QuitAction: public Action {
private:
    QuitMode m_quit_mode;
public:
    QuitAction(QuitMode quit_mode);
    QuitAction(const QuitAction&) = default;
    ~QuitAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //QUIT_ACTION_HPP
