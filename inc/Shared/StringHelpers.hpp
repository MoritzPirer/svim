///
/// @file: StringHelpers.hpp
/// @description: helper functions for working with std::strings
///
/// @date: 2026-02-02
/// @author: Moritz Pirer
///

#ifndef STRING_HELPERS_HPP
#define STRING_HELPERS_HPP

#include <string>
#include <vector>

namespace StringHelpers {

    /// @brief return a string of length total_width with str on the left, filled with pad_with. If str is 
    ///  longer than total_width, the first total_width chars of str will be chosen
    /// @param str the string to left-align
    /// @param total_width how wide the new string should be, including the padding 
    /// @param pad_with the char to fill the space with
    /// @return a string with str on the left 
    std::string leftAlign(const std::string& str, unsigned int total_width, const char& pad_with = ' ');

    std::string rightAlign(const std::string& str, unsigned int total_width, const char& pad_with = ' ');

    std::string join(const std::vector<std::string>& to_join, const std::string& join_with = ", ");

    std::string padToMulitple(const std::string& to_pad, int width, const char& pad_with = ' ');
    
    /// @brief splits the given paragraph into a vector of chunks that are at
    ///     most as long as the text area is wide (i.e that don't need to wrap)
    /// @param paragraph the paragraph to split 
    /// @param start_column where in the paragraph to start (before that column is ignored) 
    /// @return the split vector   
    std::vector<std::string> splitIntoRows(const std::string& paragraph,
        int start_column, int max_length);

    /// @brief counts the number of words in the given string, with any amount of whitespace countin
    ///     as a delimiter
    /// @param str the string to count words in
    /// @return the number of words in the string
    int countWords(const std::string& str);

    void uppercase(std::string& str);

    void lowercase(std::string& str);
}; //namespace StringHelpers

#endif //STRING_HELPERS_HPP
