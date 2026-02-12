///
/// @file: DelimiterAction.hpp
/// @description: abstract base for an action that is performed from the cursor position to the first occurrence of a delimiter
///
/// @date: 2026-02-11
/// @author: Moritz Pirer
///

#ifndef DELIMITER_ACTION_HPP
#define DELIMITER_ACTION_HPP

#include "Action.hpp"
#include "ActionOptions/EndBehavior.hpp"

class DelimiterAction: public Action {
private:
    bool isDelimiter(char c);
protected:
    std::string m_delimiters;
    Direction m_move_direction;
    EndBehavior m_end_behavior;
    bool m_paragraph_is_delimiter;
    
    /// @brief finds the position to stop at. Depending on m_end_behavior, this can be:
    ///     - the last non-delimiter before the first set of delimiters
    ///     - the first delimiter
    ///     - the first non-delimiter after the first set of delimiters
    /// @return the stop position
    Position findStopPosition(EditorState& state);

public:
    DelimiterAction(
        std::string delimiters,
        Direction move_direction,
        EndBehavior end_behavior,
        bool paragraph_is_delimiter
    );
    
    DelimiterAction(const DelimiterAction&) = default;
    ~DelimiterAction() = default;

    void applyTo(EditorState& state) override = 0;
};


#endif //DELIMITER_ACTION_HPP
