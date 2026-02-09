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
    MOVE_BY_CHARACTER,      //_hjkl
    MOVE_WITHIN_CHUNK,      //mM, aA
    MOVE_OVER_CHUNK,        //gm
    FIND,                   //fF
    FILE_ACTION,            //!
    SWITCH_MODE,            //i
    CREATE_PARAGRAPH
};

#endif //OPERATOR_TYPE_HPP
