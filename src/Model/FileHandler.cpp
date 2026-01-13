#include <fstream>
#include <filesystem>

#include "../../inc/Model/FileHandler.hpp"

TextFile FileHandler::openFile(std::string file_path) {
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

    input_file.close();
    return file;
}

void FileHandler::saveFile(const TextFile& file) {
    std::ofstream output_file(file.getFilePath());

    if (!output_file.is_open()) {
        //MODO HANDLE THIS
        return;
    }

    for (size_t i = 0; i < file.getLineCount(); i++) {
        const std::string& line = file.getLine(i);
        output_file << line; 

        if (!line.ends_with("\n") && i < file.getLineCount() - 1) {
            output_file << "\n";
        }
    }

    output_file.close();
}