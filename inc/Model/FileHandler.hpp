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
    
    TextFile openFile(const std::string& file_path);
    
    /// @brief writes the contents of file to the filename specified
    /// @param file the file to write
    void saveFile(TextFile& file);

    void renameFile(TextFile& file, std::string new_name);
};


#endif //FILE_HANDLER_HPP
