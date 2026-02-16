#include "../../../inc/Controller/Action/MessageAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

MessageAction::MessageAction(std::string message):
    m_message{message} {}

void MessageAction::apply(ExecutionContext& context) {
    context.state.addTemporaryMessage(m_message);
}