#include <iostream>
#include <fmt/format.h>
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./app <filename>\n";
        return 1;
    }

    std::vector<Command> commands;
    parse(argv[1], &commands);

    // quick verifier
    for (Command &cmd : commands) {
        std::string cmd_type;
        
        if ( cmd.cmd_type == CommandType::VAR_INIT ) {
            cmd_type = "VAR_INIT";
        }
        else if ( cmd.cmd_type == CommandType::CTRL_FLOW ) {
            cmd_type = "CTRL_FLOW";
        }
        else if ( cmd.cmd_type == CommandType::EXPRESSION ) {
            cmd_type = "EXPRESSION";
        }
        else if ( cmd.cmd_type == CommandType::SYS_FUNC ) {
            cmd_type = "SYS_FUNC";
        }
        else {
            cmd_type = "ERROR";
        }


        fmt::println("{}", cmd_type);
    }

    return 0;
}