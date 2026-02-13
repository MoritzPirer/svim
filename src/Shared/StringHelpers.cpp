#include "../../inc/Shared/StringHelpers.hpp"

using std::string, std::vector;

string StringHelpers::leftAlign(const string& str,
    unsigned int total_width, const char& pad_with) {

    if (str.length() >= total_width) {
        return str.substr(0, total_width);
    }

    unsigned int padding_needed = total_width - str.length();
    string output = str;
    output += string(padding_needed, pad_with); 

    return output; 
}

string StringHelpers::rightAlign(const string& str,
    unsigned int total_width, const char& pad_with) {

    if (str.length() >= total_width) {
        return str.substr(0, total_width);
    }

    unsigned int padding_needed = total_width - str.length();
    string output(padding_needed, pad_with);
    output += str;

    return output; 
}


string StringHelpers::join(
    const vector<string>& to_join, const string& join_with) {

    string output;

    for (size_t i = 0; i < to_join.size(); i++) {
        output += to_join.at(i);
        if (i < to_join.size() - 1) {
            output += join_with;
        }
    }

    return output;
}

string StringHelpers::padToMulitple(const string& to_pad, int width, const char& pad_with) {

    int new_length = std::ceil(to_pad.length() / static_cast<float>(width)) * width;

    return leftAlign(to_pad, new_length, pad_with);
}

vector<string> StringHelpers::splitIntoRows(const string& paragraph,
    int start_column, int max_length) {

    if (paragraph.length() == 0) {
        return {""};
    }

    vector<string> split;

    for (int i = start_column; static_cast<size_t>(i) < paragraph.length(); i += max_length) {
        split.emplace_back(paragraph.substr(i, max_length));
    }

    return split;
}

int StringHelpers::countWords(const string& str) {

    if (str.length() == 0) {
        return 0;
    } 

    auto isWhitespace = [](char c) -> bool { return string(" \t\n").find(c) != string::npos; };

    int current = 0;
    int next = 1;
    int words = (isWhitespace(str.at(0))? 0 : 1);

    while (static_cast<size_t>(next) < str.length()) {
        if (isWhitespace(str.at(current)) && !isWhitespace(str.at(next))) {
            words++;
        }

        current++;
        next++;
    }

    return words;
}