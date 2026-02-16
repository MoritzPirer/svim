#include "../../../inc/Controller/Action/FixedPositionMoveAction.hpp"
#include "../../../inc/Controller/Control/ExecutionContext.hpp"

FixedPositionMoveAction::FixedPositionMoveAction(ScreenSize text_area_size, Position target_position):
    m_text_area_size{text_area_size},
    m_target_position{target_position} {}

void FixedPositionMoveAction::apply(ExecutionContext& context) {
    //TODO: Most of this logic is for converting mouse click position to logical position
    // extract that and move it to a different spot
    EditorState& state = context.state;

    Position first_visible = state.getFirstVisibleChar(m_text_area_size);

    // how many visual lines are left in the first visible paragraph
    int lines_of_partial = TextFile::visualLinesNeeded(
        state.getParagraph(first_visible.row).length() - first_visible.column,
        m_text_area_size.width
    );

    int current_paragraph = first_visible.row;
    int visual_lines_consumed = 0;

    // handle first paragraph
    if (m_target_position.row < lines_of_partial) {
        int column =
            first_visible.column +
            m_target_position.row * m_text_area_size.width +
            m_target_position.column;

        column = std::min<int>(
            column,
            state.getParagraph(current_paragraph).length()
        );

        state.moveCursorTo({ current_paragraph, column });
        return;
    }

    visual_lines_consumed += lines_of_partial;
    current_paragraph++;

    // full paragraphs
    while (static_cast<size_t>(current_paragraph) < state.getNumberOfParagrahps()) {
        int visual_lines_of_paragraph = TextFile::visualLinesNeeded(
            state.getParagraph(current_paragraph).length(),
            m_text_area_size.width
        );

        if (visual_lines_consumed + visual_lines_of_paragraph > m_target_position.row) {
            // click is inside this paragraph
            int row_inside_paragraph =
                m_target_position.row - visual_lines_consumed;

            int column =
                row_inside_paragraph * m_text_area_size.width +
                m_target_position.column;

            column = std::min<int>(
                column,
                state.getParagraph(current_paragraph).length()
            );

            state.moveCursorTo({ current_paragraph, column });
            return;
        }

        visual_lines_consumed += visual_lines_of_paragraph;
        current_paragraph++;
    }

    // click after end of file
    int last = state.getNumberOfParagrahps() - 1;
    state.moveCursorTo({
        last,
        static_cast<int>(state.getParagraph(last).length())
    });

}