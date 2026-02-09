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

#include "../../Model/TextFile.hpp"

namespace FileHandler {

    /// @brief if a file exists at file_path, that file is read in and returned. Otherwise, an empty
    ///     TextFile is created.
    /// @param file_path the file path to use for the TextFile
    TextFile openFile(const std::string& file_path);

    /// @brief creates and returns a new TextFile object with the given file path
    TextFile createFile(std::filesystem::path file_path); 
    
    /// @brief writes the contents of file to the filename specified, creating the file if needed
    /// @param file the file to write
    void saveFile(TextFile& file);

    /// @brief updates the given file to have the new name
    /// @param file 
    /// @param new_name 
    void renameFile(TextFile& file, std::string new_name);

    /// @brief finds a file path that is not currently in use
    std::filesystem::path getDefaultName();
};


#endif //FILE_HANDLER_HPP
