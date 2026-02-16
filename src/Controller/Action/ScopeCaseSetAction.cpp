#include "../../../inc/Controller/Action/ScopeCaseSetAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

ScopeCaseSetAction::ScopeCaseSetAction(
    ScreenSize size,
    Scope scope,
    Case target_case
):
    ScopeAction(size, EndBehavior::STOP_BEFORE_END),
    m_scope{scope},
    m_target_case{target_case}
    {}

void ScopeCaseSetAction::apply(ExecutionContext& context) {
    EditorState& state = context.state;
    
    Position start_of_scope = startOfScope(state, m_scope);
    Position end_of_scope = endOfScope(state, m_scope);

    for (int row = start_of_scope.row; row <= end_of_scope.row; row++) {
        if (state.getParagraph(row).length() == 0) {
            continue;
        }

        int start_column = (row == start_of_scope.row? start_of_scope.column : 0);
        int end_column =
            (row == end_of_scope.row? end_of_scope.column : state.getParagraph(row).length() - 1);

        for (int column = start_column; column <= end_column; column++) {

            // guard against overhang cursor position
            if (static_cast<size_t>(column) >= state.getParagraph(row).length()) {
                continue;
            }

            char character = *state.readCharacterAt({row, column});

            character = (m_target_case == Case::UPPER_CASE)?
                std::toupper(character) : std::tolower(character);

            state.setCharacterAt(character, {row, column});
        }
    }
}
