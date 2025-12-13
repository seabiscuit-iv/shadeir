#include "logexpression.h"

#include <algorithm>
#include <fmt/format.h>

Comparator str_to_comparator(const std::string& comp ) {
    if ( comp == "<" ) {
        return Comparator::LESS_THAN;
    }
    else if ( comp == "<=" ) {
        return Comparator::LESS_THAN_EQ;
    }
    else if ( comp == "==" ) {
        return Comparator::EQ;
    }
    else if ( comp == "!=" ) {
        return Comparator::NOT_EQ;
    }
    else if ( comp == ">" ) {
        return Comparator::GREATER_THAN;
    }
    else if ( comp == ">=" ) {
        return Comparator::GREATER_THAN_EQ;
    }
    else {
        fmt::println("Comparator parsing error: {} is not a comparator", comp);
        exit(1);
    }
}

LogExpression str_to_expression(const std::string& left, const std::string& comp, const std::string& right, const std::vector<Variable>& var_list) {
    LogExpression exp;

    Comparator c = str_to_comparator(comp);

    exp.left_is_literal = is_number(left);
    exp.right_is_literal = is_number(right);

    if ( exp.left_is_literal ) {
        exp.l_left.l_int = std::stoi(left);
    }
    else {
        auto find_left = std::find_if(var_list.begin(), var_list.end(), [&](const Variable& var){
            return var.name == left;
        });

        if (find_left == var_list.end()) {
            fmt::println("Undeclared variable {}", left);
            exit(1);
        }

        exp.left = *find_left;
    }

    if ( exp.right_is_literal ) {
        exp.l_right.l_int = std::stoi(right);
    }
    else {
        auto find_right = std::find_if(var_list.begin(), var_list.end(), [&](const Variable& var){
            return var.name == right;
        });

        if (find_right == var_list.end()) {
            fmt::println("Undeclared variable {}", right);
            exit(1);
        }

        exp.right = *find_right;
    }
    
    exp.comp = c;

    return exp;
}

std::string comparator_to_ptx(Comparator cmp) {
    if (cmp == Comparator::EQ) {
        return "eq";
    }
    else if (cmp == Comparator::NOT_EQ) {
        return "ne";
    }
    else if (cmp == Comparator::LESS_THAN) {
        return "lt";
    }
    else if (cmp == Comparator::LESS_THAN_EQ) {
        return "le";
    }
    else if (cmp == Comparator::GREATER_THAN) {
        return "gt";
    }
    else if (cmp == Comparator::GREATER_THAN_EQ) {
        return "ge";
    }
    else {
        fmt::println("Unreachable!");
        exit(1);
    }
}