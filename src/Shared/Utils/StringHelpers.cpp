#include "../../../inc/Shared/Utils/StringHelpers.hpp"

using std::string, std::vector;

string StringHelpers::leftAlign(const string& str, unsigned int total_width, char pad_with) {

    if (str.length() >= total_width) {
        return str.substr(0, total_width);
    }

    unsigned int padding_needed = total_width - str.length();
    string output = str;
    output += string(padding_needed, pad_with); 

    return output; 
}

string StringHelpers::rightAlign(const string& str, unsigned int total_width, char pad_with) {

    if (str.length() >= total_width) {
        return str.substr(0, total_width);
    }

    unsigned int padding_needed = total_width - str.length();
    string output(padding_needed, pad_with);
    output += str;

    return output; 
}

string StringHelpers::addSeperators(int number, int group_size, char seperator) {
    string str = std::to_string(number);

    for (int i = static_cast<int>(str.size()) - group_size; i > 0; i -= group_size) {
        str.insert(str.begin() + i, seperator);
    }

    return str;
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

void StringHelpers::uppercase(std::string& str) {
    for (char& c : str) {
        c = std::toupper(c);
    }
}

void StringHelpers::lowercase(std::string& str) {
    for (char& c : str) {
        c = std::tolower(c);
    }
}

std::optional<char> StringHelpers::firstNonSpace(const std::string& str) {
    size_t index = str.find_first_not_of(' ');

    if (index == std::string::npos) {
        return std::nullopt;
    }

    return str.at(index);
}

bool StringHelpers::startsWith(const std::string& str, char first) {
    if (str.empty()) {
        return false;
    }

    return str.at(0) == first;
}

bool StringHelpers::startsWithIgnoringWhitespace(const std::string& str, char first) {
    size_t index = str.find_first_not_of(' ');

    if (index == std::string::npos) {
        return false;
    }

    return str.at(index) == first;
}

bool StringHelpers::consistsOnlyOfIgnoringWhitespace(const std::string& str, char comparison) {
    for (char c : str) {
        if (c != comparison && c != ' ') {
            return false;
        }
    }

    return true;
}