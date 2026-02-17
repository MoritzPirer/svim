#include "../../../inc/Shared/StringHelpers.hpp"
#include "../../../inc/Controller/Action/CaseSetAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

CaseSetAction::CaseSetAction(
    Position start,
    Position end,
    Case target_case
):
    RangedAction(start, end),
    m_target_case{target_case},
    m_original_content{}
    {}

std::vector<std::string> CaseSetAction::getContentAsCase() {
    std::vector<std::string> changed_content = m_original_content;

    for (std::string& row : changed_content) {
        if (m_target_case == Case::UPPER_CASE) {
            StringHelpers::uppercase(row);
        }
        else {
            StringHelpers::lowercase(row);
        }
    }

    return changed_content;
}

void CaseSetAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;

    m_original_content = state.copyRange(m_start, m_end);
    state.deleteRange(m_start, m_end);

    std::vector<std::string> changed_content = getContentAsCase();
    state.insertLines(changed_content, m_start);
}

void CaseSetAction::undo(EditorState& state) {
    state.deleteRange(m_start, m_end);
    state.insertLines(m_original_content, m_start);
}

bool CaseSetAction::canBeUndone() const {
    return true;
}