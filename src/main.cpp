#include <iostream>
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./app <filename>\n";
        return 1;
    }

    std::vector<Command> commands;
    parse(argv[1], &commands);
    return 0;
}