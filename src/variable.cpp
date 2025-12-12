#include "variable.h"

std::vector<Variable> get_variable_list( const std::vector<Command>& commands ) 
{
    std::vector<Variable> variables;
    for (const Command& cmd : commands) {  
        if (cmd.cmd_type == CommandType::VAR_INIT) {
            variables.push_back( Variable { cmd.var_init.name, cmd.var_init.dataType });
        }
    }

    return variables;
}