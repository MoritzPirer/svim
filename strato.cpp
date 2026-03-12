#include "inc/Controller/EditorController.hpp"

int main(int argument_count, char* argument_values[]) { 
    switch (argument_count) {
    case 1: {
        EditorController controller;
        controller.mainLoop(argument_values[0]);
        break;
    }

    case 2: {
        EditorController controller(argument_values[1]);
        controller.mainLoop(argument_values[0]);
        break;
    }

    }

    return 0;
}