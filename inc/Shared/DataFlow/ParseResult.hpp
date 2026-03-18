///
/// @file: ParseResult.hpp
/// @description: the result of parsing an input
///
/// @date: 2026-02-06x
/// @author: Moritz Pirer
///

#ifndef PARSE_RESULT_HPP
#define PARSE_RESULT_HPP

#include <memory>
#include <vector>

#include "../../Controller/Modes/ModeType.hpp"
#include "../../Controller/Actions/Action.hpp"

struct ParseResult {
    std::optional<ModeType> mode;
    std::optional<std::shared_ptr<Action>> action;    

    static ParseResult nullObject() { return {std::nullopt, std::nullopt}; }
};

#endif //PARSE_RESULT_HPP
