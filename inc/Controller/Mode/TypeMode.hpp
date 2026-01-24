///
/// @file: TypeMode.hpp
/// @description: description
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef TYPE_MODE_HPP
#define TYPE_MODE_HPP

#include "EditorMode.hpp"

class TypeMode: public EditorMode {
private:

public:
    TypeMode() = default;
    TypeMode(const TypeMode&) = default;
    ~TypeMode() = default;

    std::pair<ModeType, std::shared_ptr<Action>>  parseInput(int input) override;
    std::string getModeLabel() const override { return "TYPE"; }
};

#endif //TYPE_MODE_HPP
