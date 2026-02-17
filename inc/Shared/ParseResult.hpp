///
/// @file: ParseResult.hpp
/// @description: the result of parsing an input
///
/// @date: 2026-02-06
/// @author: Moritz Pirer
///

#ifndef PARSE_RESULT_HPP
#define PARSE_RESULT_HPP

#include <memory>
#include <vector>

#include "../Controller/Mode/ModeType.hpp"
#include "../Controller/Action/Action.hpp"

struct ParseResult {
    std::optional<ModeType> mode;
    std::optional<std::shared_ptr<Action>> action;    
};

#endif //PARSE_RESULT_HPP
