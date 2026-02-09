///
/// @file: CommandDetails.hpp
/// @description: description
///
/// @date: 2026-02-08
/// @author: Moritz Pirer
///

#ifndef COMMAND_DETAILS_HPP
#define COMMAND_DETAILS_HPP

#include "Operator.hpp"
#include "../../Shared/Direction.hpp"
#include "../../Shared/Scope.hpp"
#include "../Mode/ModeType.hpp"

struct CommandDetails {
    Operator operator_type;
    Direction direction;
    std::optional<Scope> scope;
    std::optional<char> argument;
    ModeType next_mode;

    bool is_complete;
};

#endif //COMMAND_DETAILS_HPP
