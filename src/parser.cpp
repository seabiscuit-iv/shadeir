#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <regex>
#include "parser.h"
#include <fmt/format.h>
#include <algorithm>


[[noreturn]]
inline void syntax_err(std::string s, int line_number) {
    fmt::print(fmt::format("Syntax error on line {}\n{}", line_number, s));
    exit(1);
}


void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}


void parse(const std::string& filename, std::vector<Command>* commands) {
    std::ifstream file(filename);

    // Check file can be opened
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;
    int lineNumber = 1;

    while (std::getline(file, line)) {

        trim(line);
        
        std::smatch match;
        Command cmd;

        if (std::regex_match(line, match, var_init_pattern)) {
            cmd = handle_var_init(match, lineNumber);
        } 
        else if (std::regex_match(line, match, control_pattern)) {
            cmd = handle_ctrl_flow(match, lineNumber);
        } 
        else if (std::regex_match(line, match, expression_pattern)) {
            cmd = handle_expression(match, lineNumber);
        } 
        else if (std::regex_match(line, match, sys_func_pattern)) {
            cmd = handle_sysfunc(match, lineNumber);
        }
        else if (line.empty()) {
            lineNumber += 1;
            continue;
        }
        else {
            std::cerr << "Error 5: Syntax error on line " << lineNumber << "\n";
            return;
        }
        
        commands->push_back(cmd);
        lineNumber += 1;
    }

    file.close();
    return;
}


Command handle_var_init(const std::smatch& match, int line_number) {
    Command cmd;
    cmd.cmd_type = CommandType::VAR_INIT;

    std::string descriptor = match[1];
    std::string type       = match[2];
    cmd.var_init.name      = match[3];

    if (descriptor == "vector") {
        cmd.var_init.descType = VarDescType::VECTOR;
    } 
    else if (descriptor == "scalar") {
        cmd.var_init.descType = VarDescType::SCALAR;
    } 
    else {
        syntax_err(fmt::format("Available variable descriptor types are [vector] and [scalar], not [{}]", descriptor), line_number);
    }

    if (type == "i32") {
        cmd.var_init.dataType = VarDataType::INTEGER;
    } else if (type == "f32") {
        cmd.var_init.dataType = VarDataType::FLOAT;
    } else {
        syntax_err(fmt::format("Available variable data types are i32 and f32, not {}", type), line_number);
    }
    return cmd;
}


Command handle_ctrl_flow(const std::smatch& match, int line_number) {
    Command cmd;
    cmd.cmd_type = CommandType::CTRL_FLOW;

    std::string c_flow = match[1]; 
    if (match[2].matched) {
        cmd.ctrl_flow.conditional = match[2];
    }

    if (ctrl_flows_map.count(c_flow)) {
        cmd.ctrl_flow.flowCmdType = ctrl_flows_map.at(c_flow);
    } 
    else {
        syntax_err(fmt::format("No control flow style argument #{}", c_flow), line_number);
    }

    return cmd;
}

Command handle_expression(const std::smatch& match, int line_number) {
    Command cmd;
    cmd.cmd_type = CommandType::EXPRESSION;

    std::string c = match[1];
    std::string a = match[2];
    std::string op = match[3];
    std::string b = match[4];

    cmd.expression.a = a;
    cmd.expression.b = b;
    cmd.expression.c = c;

    if (operator_map.count(op)) {
        cmd.expression.op = operator_map.at(op);
    } 
    else {
        syntax_err(fmt::format("No expression operator {}", op), line_number);
    }

    return cmd;
}

Command handle_sysfunc(const std::smatch& match, int line_number) {
    Command cmd;
    cmd.cmd_type = CommandType::SYS_FUNC;

    std::string func = match[1]; 
    std::string a = match[2];    
    std::string b = match[3];    

    cmd.sys_func.function = func;
    cmd.sys_func.input_a = a;
    cmd.sys_func.input_b = b;

    return cmd;
}