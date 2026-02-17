#include <fstream>
#include <filesystem>

#include "../../../inc/Controller/Control/FileException.hpp"
#include "../../../inc/Controller/Control/FileHandler.hpp"

using std::string;
namespace {

    string constructDefaultFilename(int counter) {
        string default_file_name = "new_file";
        string default_file_ending = ".txt";

        string numbered_name;
        if (counter == 0) {
            return default_file_name + default_file_ending;
        }
        
        return default_file_name + "_(" + std::to_string(counter) + ")" + default_file_ending;
    }

}


TextFile FileHandler::openFile(const string& file_path) {
    
    if (!std::filesystem::exists(file_path)) {
        return createFile(file_path);
    }
    
    TextFile file(file_path, SaveState::SAVED);
    std::ifstream input_file(file_path);
    if (!input_file.is_open()) {
        //MODO HANDLE THIS
        return file;
    }

    string line;

    while (getline(input_file, line)) {
        file.writeToEnd(line);
    }

    file.markAsSaved();

    input_file.close();
    return file;
}

TextFile FileHandler::createFile(std::filesystem::path file_path) {
    TextFile file(file_path, SaveState::NEW_FILE);
    file.writeToEnd("");
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
            throw FileException();
        }
    
        for (int i = 0; i < file.getNumberOfParagrahps(); i++) {
            const string& line = file.getParagraph(i);
            output_file << line; 
    
            if (!line.ends_with("\n") && i < file.getNumberOfParagrahps() - 1) {
                output_file << "\n";
            }
        }

        output_file.close();
        file.markAsSaved();
    }
    catch (const std::filesystem::filesystem_error& e) {
        throw FileException();
    }
}

void FileHandler::renameFile(TextFile& file, string new_path) {
    std::filesystem::path new_file_path(new_path);

    if (new_file_path.is_absolute()) {
        file.setFilepath(new_file_path);
    }
    else {
        std::filesystem::path base_directory = file.getFilepath().parent_path();
        file.setFilepath(base_directory/new_path); // '/' operator is concatination here
    }
}

std::filesystem::path FileHandler::getDefaultName() {
    int counter = 0;

    while (true) {
        string default_name = constructDefaultFilename(counter);
        std::filesystem::path default_path = std::filesystem::absolute(default_name);

        if (!std::filesystem::exists(default_path)) {
            return default_path;
        }

        counter++;
    }
}