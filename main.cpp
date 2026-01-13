#include "inc/Model/FileHandler.hpp"

int main(int arguments_count, char* arguments_list[]) {
    FileHandler fh;
    TextFile file = fh.openFile("test.txt");
    file.writeToEnd("THIS WAS WRITTEN BY THE PROGRAM!");
    fh.saveFile(file);
    return 0;
}