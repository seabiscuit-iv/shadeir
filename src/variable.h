#pragma once

#include <string>
#include "parser.h"

struct Variable {
    std::string name;
    VarDataType data_type;
};

std::vector<Variable> get_variable_list( const std::vector<Command>& commands );