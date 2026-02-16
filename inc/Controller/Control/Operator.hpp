///
/// @file: Operator.hpp
/// @description: description
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef OPERATOR_TYPE_HPP
#define OPERATOR_TYPE_HPP

enum class Operator {
    MOVE_BY_CHARACTER,      //hjkl
    MOVE_WITHIN_CHUNK,      //mM, aA
    MOVE_OVER_CHUNK,        //gm
    MOVE_FIND,              //fF

    PARAGRAPH_JOIN,         //+
    PARAGRAPH_SPLIT,        //-
    PARAGRAPH_CREATE,       //oO

    ERASE,                  //e
    REPLACE,                //r
    INDENT,                 //>
    UNINDENT,               //<
    CASE_SET_LOWER,         //t
    CASE_SET_UPPER,         //T

    FILE_ACTION,            //!
    SWITCH_MODE,            //i
    UNDO
};

#endif //OPERATOR_TYPE_HPP
