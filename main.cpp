#include "inc/View/Display.hpp"

int main(int argument_count, char* argument_values[]) { 
    switch (argument_count) {
        case 1: {
            Display d;
            d.mainLoop();
            break;
        }
        case 2: {
            Display d(argument_values[1]);
            d.mainLoop();
            break;
        }
    }

    return 0;
}