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

#include "../../Model/TextFile.hpp"

class FileHandler {
    std::string constructDefaultFilename(int counter);
public:
    
    TextFile openFile(const std::string& file_path);
    TextFile createFile(std::filesystem::path file_path); 
    
    /// @brief writes the contents of file to the filename specified
    /// @param file the file to write
    void saveFile(TextFile& file);

    void renameFile(TextFile& file, std::string new_name);

    std::filesystem::path getDefaultName();
};


#endif //FILE_HANDLER_HPP
