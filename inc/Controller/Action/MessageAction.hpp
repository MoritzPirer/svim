///
/// @file: MessageAction.hpp
/// @description: describes an action that moves the cursor to a given position
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef MESSAGE_ACTION_HPP
#define MESSAGE_ACTION_HPP

#include "Action.hpp"
#include "../../Shared/Direction.hpp"

class MessageAction: public Action {
private:
    std::string m_message;
public:
    MessageAction(std::string message);
    MessageAction(const MessageAction&) = default;
    ~MessageAction() = default;

    void apply(ExecutionContext& context) override;
};

#endif //MESSAGE_ACTION_HPP
