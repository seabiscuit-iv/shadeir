#pragma once

#include "variable.h"

#include <string>

enum class Comparator {
    LESS_THAN,
    LESS_THAN_EQ,
    EQ,
    NOT_EQ,
    GREATER_THAN,
    GREATER_THAN_EQ
};

struct LogExpression {
    Variable left;
    Variable right;

    Literal l_left;
    Literal l_right;

    bool left_is_literal;
    bool right_is_literal;

    Comparator comp;
};


Comparator str_to_comparator(const std::string& comp );

LogExpression str_to_expression(const std::string& left, const std::string& comp, const std::string& right, const std::vector<Variable>& var_list);

std::string comparator_to_ptx(Comparator cmp);

const inline bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
