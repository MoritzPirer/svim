#include <fstream>
#include <filesystem>

#include "../../inc/Model/FileHandler.hpp"

TextFile FileHandler::openFile(const std::string& file_path) {
    TextFile file(file_path);

    if (!std::filesystem::exists(file_path)) {
        return file;
    }

    std::ifstream input_file(file_path);
    if (!input_file.is_open()) {
        //MODO HANDLE THIS
        return file;
    }

    std::string line;

    while (getline(input_file, line)) {
        file.writeToEnd(line);
    }
    fprintf(stderr, "read file");
    fflush(stderr);
    file.calculateMetadata();

    input_file.close();
    return file;
}

void FileHandler::saveFile(TextFile& file) {
    std::filesystem::path file_path = file.getFilepath();
    std::filesystem::path parent_directory = file_path.parent_path();

    try {
        if (!parent_directory.empty()) {
            std::filesystem::create_directories(parent_directory);
        }

        std::ofstream output_file(file.getFilepath().string());
    
        if (!output_file.is_open()) {
            //MODO HANDLE THIS
            return;
        }
    
        for (int i = 0; i < file.getLineCount(); i++) {
            const std::string& line = file.getLine(i);
            output_file << line; 
    
            if (!line.ends_with("\n") && i < file.getLineCount() - 1) {
                output_file << "\n";
            }
        }

        file.setHasUnsavedChanges(false);

        output_file.close();
    }
    catch (const std::filesystem::filesystem_error& e) {
        //MODO HANDLE THIS
        return;
    }
}

void FileHandler::renameFile(TextFile& file, std::string new_path) {
    std::filesystem::path new_file_path(new_path);

    if (new_file_path.is_absolute()) {
        file.setFilepath(new_file_path);
    }
    else {
        std::filesystem::path base_directory = file.getFilepath().parent_path();
        file.setFilepath(base_directory/new_path); // '/' operater is concatination here
    }
}