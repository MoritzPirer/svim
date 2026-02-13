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
    bool isAntiDelimiter(char c);

    bool isSuitableEndpoint(EditorState& state, char character, bool& has_reached_delimiter,
        bool& has_reached_non_delimiter, int& delimiter_balance);

protected:
    std::string m_delimiters;
    std::string m_anti_delimiters;
    Direction m_move_direction;
    EndBehavior m_end_behavior;
    bool m_paragraph_is_delimiter;
    
    Position findStopPosition(EditorState& state);

public:
    DelimiterAction(
        std::string delimiters,
        std::string anti_delimiters,
        Direction move_direction,
        EndBehavior end_behavior,
        bool paragraph_is_delimiter
    );
    
    DelimiterAction(const DelimiterAction&) = default;
    ~DelimiterAction() = default;

    void applyTo(EditorState& state) override = 0;
};


#endif //DELIMITER_ACTION_HPP
