#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <regex>
#include "parser.h"

std::unordered_map<std::string, FlowCmdType> ctrl_flows_map = {
    {"while_exec", FlowCmdType::WHILE_EXEC},
    {"end_exec", FlowCmdType::END_EXEC},
    {"exec_mask", FlowCmdType::EXEC_MASK}
};

std::unordered_map<std::string, OperatorType> operator_map = {
    {"+", OperatorType::ADD},
    {"-", OperatorType::SUBTRACT},
    {"*", OperatorType::MULTIPLY},
    {"/", OperatorType::DIVIDE},
    {"%", OperatorType::MODULUS}
};

void parse(const std::string& filename, std::vector<Command>* commands) {
    std::ifstream file(filename);

    // Check file can be opened
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;

    std::regex var_init_pattern(
        "\\[([^\\]]+)\\]\\s+([A-Za-z_]\\w*)\\s+([A-Za-z_]\\w*)\\s*=\\s*(.+)"
    );
    std::regex control_pattern("^#(\\w+)(?:\\s+(.*))?$");
    std::regex expression_pattern(
        "^\\s*([A-Za-z0-9_]+)\\s*"
        "(?:"
            "=\\s*([A-Za-z0-9_]+)\\s*([+\\-*/%&|^<>]+)\\s*([A-Za-z0-9_]+)"
            "|"
            "([+\\-*/%&|^<>]+)=\\s*([A-Za-z0-9_]+)"
        ")\\s*$"
    );
    std::regex sys_func_pattern("^@([A-Za-z0-9_()]+)\\s*<=\\s*([A-Za-z0-9_()]+)");

    int lineNumber = 1;

    while (std::getline(file, line)) {
        
        std::smatch match;
        Command cmd;

        if (std::regex_match(line, match, var_init_pattern)) {
            cmd.cmd_type = CommandType::VAR_INIT;

            std::string descriptor = match[1];
            std::string type       = match[2];
            cmd.var_init.name      = match[3];
            cmd.var_init.value     = match[4];

            if (descriptor == "vector") {
                cmd.var_init.descType = VarDescType::VECTOR;
            } else if (descriptor == "scalar") {
                cmd.var_init.descType = VarDescType::SCALAR;
            } else {
                std::cerr << "Error 1: Syntax error on line " << lineNumber << "\n";
                return;
            }

            if (type == "i32") {
                cmd.var_init.dataType = VarDataType::INTEGER;
            } else if (type == "f32") {
                cmd.var_init.dataType = VarDataType::FLOAT;
            } else {
                std::cerr << "Error 2: Syntax error on line " << lineNumber << "\n";
                return;
            }
        } else if (std::regex_match(line, match, control_pattern)) {
            cmd.cmd_type = CommandType::CTRL_FLOW;
            if (match[2].matched) {
                cmd.ctrl_flow.conditional = match[2];
            }
            std::string c_flow = match[1];

            if (ctrl_flows_map.count(c_flow)) {
                cmd.ctrl_flow.flowCmdType = ctrl_flows_map[c_flow];
            } else {
                std::cerr << "Error 3: Syntax error on line " << lineNumber << "\n";
                return;
            }
        } else if (std::regex_match(line, match, expression_pattern)) {
            cmd.cmd_type = CommandType::EXPRESSION;
            cmd.expression.c = match[1];
            std::string op;
            
            if (match[2].matched) {
                cmd.expression.a = match[2];
                cmd.expression.b = match[4];

                op = match[3];
            } else {
                cmd.expression.a = match[6];

                op = match[5];
            }

            if (operator_map.count(op)) {
                cmd.expression.op = operator_map[op];
            } else {
                std::cerr << "Error 4: Syntax error on line " << lineNumber << "\n";
                return;
            }
        } else if (std::regex_match(line, match, sys_func_pattern)) {
            cmd.cmd_type = CommandType::SYS_FUNC;
            cmd.sys_func.function = match[1];
            cmd.sys_func.input = match[2];
        } else {
            std::cerr << "Error 5: Syntax error on line " << lineNumber << "\n";
            return;
        }
        
        commands->push_back(cmd);
        lineNumber += 1;
    }

    file.close();
    return;
}