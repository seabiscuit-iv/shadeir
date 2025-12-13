#include "variable.h"
#include <fmt/format.h>

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

const Variable* find_var( const std::string& name, const std::vector<Variable>& var_list ) {
    auto find_left = std::find_if(var_list.begin(), var_list.end(), [&](const Variable& var){
        return var.name == name;
    });

    if (find_left == var_list.end()) {
        fmt::println("Undeclared variable {}", name);
        exit(1);
    }

    return &*find_left;
}