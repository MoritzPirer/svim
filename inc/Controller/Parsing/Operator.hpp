///
/// @file: Operator.hpp
/// @description: describes the different operators the editor knows
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef OPERATOR_TYPE_HPP
#define OPERATOR_TYPE_HPP

enum class Operator {
    MOVE_BY_CHARACTER,      //hjkl
    MOVE_TO_END,      //mM, aA
    MOVE_TO_NEXT,        //gm
    MOVE_TO_FIND,              //fF

    PARAGRAPH_JOIN,         //+
    PARAGRAPH_SPLIT,        //-
    PARAGRAPH_CREATE,       //oO

    DELETE_SINGLE,                  //e
    DELETE_WITHIN,
    DELETE_UNTIL,
    REPLACE,                //r
    INDENT,                 //>
    UNINDENT,               //<
    CASE_SET_LOWER,         //t
    CASE_SET_UPPER,         //T

    FILE_ACTION,            //!
    SWITCH_MODE,            //i
    UNDO,
    REDO
};

#endif //OPERATOR_TYPE_HPP
