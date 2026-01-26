///
/// @file: InsertAction.hpp
/// @description: description
///
/// @date: 2026-01-24
/// @author: Moritz Pirer
///

#ifndef INSERT_ACTION_HPP
#define INSERT_ACTION_HPP

#include "Action.hpp"

class InsertAction: public Action {
private:
    char m_character_to_add;
public:
    InsertAction(char character_to_add);
    InsertAction(const InsertAction&) = default;
    ~InsertAction() = default;

    void applyTo(EditorState& state) override;
};

#endif //INSERT_ACTION_HPP
