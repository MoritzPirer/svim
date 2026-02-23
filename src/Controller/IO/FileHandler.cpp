#include <fstream>
#include <filesystem>

#include "../../../inc/Controller/IO/FileException.hpp"
#include "../../../inc/Controller/IO/FileHandler.hpp"

using std::string, std::filesystem::path;
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

    std::ofstream openOutputFile(const TextFile& file) {
        path file_path = file.getFilepath();
        // Use absolute path to ensure we aren't losing the directory context
        path abs_path = std::filesystem::absolute(file_path);
        path parent_directory = abs_path.parent_path();

        if (!parent_directory.empty() && !std::filesystem::exists(parent_directory)) {
            std::filesystem::create_directories(parent_directory);
        }

        std::ofstream output_file(abs_path.string());
        if (!output_file.is_open()) {
            throw FileException("Unable to open file "); //'" + abs_path.string() + "'! Error: " + strerror(errno));
        }
        return output_file; // Should move correctly in C++20
    }
    // std::ofstream openOutputFile(const TextFile& file) {

    //     path file_path = file.getFilepath();
    //     path parent_directory = file_path.parent_path();

    //     if (!parent_directory.empty()) {
    //         std::filesystem::create_directories(parent_directory);
    //     }

    //     std::ofstream output_file(file.getFilepath().string());
    
    //     if (!output_file.is_open()) {
    //         throw FileException("Unable to open file '" + file_path.string() + "'!");
    //     }

    //     return output_file;
    // }

    void writeToFile(std::ofstream& output_file, TextFile& file) {
        for (int i = 0; i < file.getNumberOfParagrahps(); i++) {
            const string& line = file.getParagraph(i);
            output_file << line; 
    
            if (!line.ends_with("\n") && i < file.getNumberOfParagrahps() - 1) {
                output_file << "\n";
            }
        }

        file.markAsSaved();
    }

    TextFile readFromFile(std::ifstream& input_file, const string& file_path) {
        TextFile file(file_path, SaveState::SAVED);
        string line;

        while (getline(input_file, line)) {
            file.writeToEnd(line);
        }

        file.markAsSaved();
        return file;
    }

} // anonymous namespace


TextFile FileHandler::openFile(const string& file_path) {
    
    std::filesystem::path absolute = std::filesystem::absolute(file_path);
    if (!std::filesystem::exists(absolute)) {
        return createFile(absolute);
    }
    
    std::ifstream input_file(absolute);
    if (!input_file.is_open()) {
        //MODO HANDLE THIS
        throw FileException("Unable to open input file!");
        return {file_path, SaveState::SAVED};
    }

    TextFile file = readFromFile(input_file, absolute);
    input_file.close();

    return file;
}

TextFile FileHandler::createFile(path file_path) {
    TextFile file(file_path, SaveState::NEW_FILE);
    file.writeToEnd("");
    return file;
}


void FileHandler::saveFile(TextFile& file) {
    try {
        std::ofstream output_file = openOutputFile(file);
        writeToFile(output_file, file); 
        output_file.close();
    }
    catch (const std::filesystem::filesystem_error& e) {
        throw FileException("A filesytem error occurred when saving!");
    }
}

void FileHandler::renameFile(TextFile& file, string new_path) {
    path new_file_path(new_path);

    if (new_file_path.is_absolute()) {
        file.setFilepath(new_file_path);
    }
    else {
        path base_directory = file.getFilepath().parent_path();
        file.setFilepath(base_directory/new_path); // '/' operator is concatination here
    }
}

path FileHandler::getDefaultName() {
    int counter = 0;

    while (true) {
        string default_name = constructDefaultFilename(counter);
        path default_path = std::filesystem::absolute(default_name);

        if (!std::filesystem::exists(default_path)) {
            return default_path;
        }

        counter++;
    }
}

path FileHandler::createBackupLocation(path executable_path) {
    std::filesystem::path backup_dir = executable_path.parent_path() / "backups"; 
    std::filesystem::create_directories(backup_dir);

    return backup_dir;
}

path FileHandler::getBackupPath(path file_path, path backup_directory) {
    std::string backup_filename = file_path.stem().string()
        + " (backup)" + file_path.extension().string();

    return backup_directory / backup_filename;
}
