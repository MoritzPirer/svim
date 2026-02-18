///
/// @file: CommandDetails.hpp
/// @description: Stores partial results of command parsing
///
/// @date: 2026-02-08
/// @author: Moritz Pirer
///

#ifndef COMMAND_DETAILS_HPP
#define COMMAND_DETAILS_HPP

#include <optional>

#include "Operator.hpp"
#include "../../Shared/Types/Direction.hpp"
#include "../../Shared/Types/Scope.hpp"
#include "../Modes/ModeType.hpp"

struct CommandDetails {
    Operator operator_type;
    Direction direction;
    std::optional<Scope> scope;
    std::optional<char> argument;
    ModeType next_mode;

    bool is_complete;
};

#endif //COMMAND_DETAILS_HPP
