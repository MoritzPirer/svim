///
/// @file: FileHandler.hpp
/// @description: description
///
/// @date: 2026-01-13
/// @author: Moritz_Pirer
///

#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include <optional>

#include "TextFile.hpp"

class FileHandler {
public:
    
    TextFile openFile(std::string file_path);
    
    void saveFile(const TextFile& file);
};


#endif //FILE_HANDLER_HPP
