///
/// @file: TextStyle.hpp
/// @description: description
///
/// @date: 2026-03-10
/// @author: Moritz Pirer
///

#ifndef TEXT_STYLE_HPP
#define TEXT_STYLE_HPP

struct TextStyle {
    bool is_bold;
    bool is_italic;

    TextStyle(bool bold, bool italic):
        is_bold{bold},
        is_italic{italic}
        {}
    
    static TextStyle makeNormal() { return TextStyle{false, false}; }

    static TextStyle makeBold() { return TextStyle{true, false}; }

    static TextStyle makeItalic() { return TextStyle{false, true}; }
    
    static TextStyle makeBoldItalic() { return TextStyle{true, true}; }

    void toggleBold() { is_bold = !is_bold; }
    void toggleItalic() { is_italic = !is_italic; }
};

#endif //TEXT_STYLE_HPP
