///
/// @file: SmartList.hpp
/// @description: functions for automatically inserting bullet points, checkboxes, nubered lists etc
///
/// @date: 2026-03-18
/// @author: Moritz Pirer
///

#ifndef SMART_LIST_HPP
#define SMART_LIST_HPP

#include "ParsingContext.hpp"
#include "../../Shared/DataFlow/ParseResult.hpp"

namespace SmartList {
    ParseResult trySmartListInsertion(ParsingContext context);
}; //namespace SmartList

#endif //SMART_LIST_HPP
