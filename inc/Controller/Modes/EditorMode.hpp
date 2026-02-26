///
/// @file: EditorMode.hpp
/// @description: abstract template for a mode the editor can be in
///
/// @date: 2026-01-23
/// @author: Moritz Pirer
///

#ifndef EDITOR_MODE_HPP
#define EDITOR_MODE_HPP

#include <string>
#include <memory>
#include <vector>

#include "../Actions/Action.hpp"
#include "../../Shared/DataFlow/ParseResult.hpp"
#include "../Services/Settings/Settings.hpp"
#include "../../Shared/Types/Input.hpp"
#include "../Parsing/ParsingContext.hpp"

class EditorMode {
private:
    bool isClickPositionValid(Position click_position,
        int aside_width, ScreenSize text_area_size);

    Position convertToLogicPosition(Position click_position, ParsingContext context);

protected:
    ParseResult parseMouseMovement(Position click_position, ParsingContext context);
public:
    EditorMode() = default;
    EditorMode(const EditorMode&) = default;
    virtual ~EditorMode() = default;

    virtual ParseResult parseInput(Input input, ParsingContext context) = 0;

    /// @brief returns the name of the mode for displaying
    virtual std::string getModeLabel() const = 0;
};

#endif //EDITOR_MODE_HPP
