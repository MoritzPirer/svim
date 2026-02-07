///
/// @file: OperatorType.hpp
/// @description: description
///
/// @date: 2026-02-07
/// @author: Moritz Pirer
///

#ifndef OPERATOR_TYPE_HPP
#define OPERATOR_TYPE_HPP

enum class OperatorType {
    MOVE_BY_CHARACTER,      //_hjkl
    MOVE_BY_CHUNK,          //gG
    MOVE_WITHIN_CHUNK,      //mM
    FIND,                   //fF
    FILE_ACTION,            //!
    SWITCH_MODE,            //i
    CREATE_PARAGRAPH
};

#endif //OPERATOR_TYPE_HPP
