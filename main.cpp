#include "inc/View/Display.hpp"
int main() { //int arguments_count, char* arguments_list[]) {
    // FileHandler fh;
    // TextFile file = fh.openFile("test.txt");
    // file.printMetadata();
    // file.writeToEnd("THIS WAS WRITTEN BY THE PROGRAM!");
    // file.printMetadata();
    // fh.renameFile(file, "renamed.txt");
    // fh.saveFile(file);
    Display d;
    d.mainLoop();
    return 0;
}