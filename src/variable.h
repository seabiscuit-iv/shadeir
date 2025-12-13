#pragma once

#include <string>
#include "parser.h"

struct Variable {
    std::string name;
    VarDataType data_type;
};

struct Literal {
    int l_int;
    float l_float;
};

std::vector<Variable> get_variable_list( const std::vector<Command>& commands );

const Variable* find_var( const std::string& name, const std::vector<Variable>& vars );