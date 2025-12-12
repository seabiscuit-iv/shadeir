#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <regex>

enum class CommandType {
    VAR_INIT,
    CTRL_FLOW,
    EXPRESSION,
    SYS_FUNC
};

enum class VarDescType {
    VECTOR,
    SCALAR
};

enum class VarDataType {
    FLOAT,
    INTEGER
};

enum class FlowCmdType {
    WHILE_EXEC,
    EXEC_MASK,
    END_EXEC
};

enum class OperatorType {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULUS
};

enum class SysFuncType {
    WRITEBUF,
    LOADBUF
};

struct VariableInitializer {
    VarDescType descType;
    VarDataType dataType;
    std::string name;
};

struct ControlFlowCommand {
    FlowCmdType flowCmdType;
    std::string left;
    std::string right;
    std::string comp;
};

struct Expression {
    OperatorType op;
    std::string a;
    std::string b;
    std::string c;
};

struct SystemFunction {
    std::string function;
    std::string input_a;
    std::string input_b;
};

struct Command {
    CommandType cmd_type;
    VariableInitializer var_init;
    ControlFlowCommand ctrl_flow;
    Expression expression;
    SystemFunction sys_func;
};

const std::unordered_map<std::string, FlowCmdType> ctrl_flows_map = {
    {"while_exec", FlowCmdType::WHILE_EXEC},
    {"end_exec", FlowCmdType::END_EXEC},
    {"exec_mask", FlowCmdType::EXEC_MASK}
};

const std::unordered_map<std::string, OperatorType> operator_map = {
    {"+", OperatorType::ADD},
    {"-", OperatorType::SUBTRACT},
    {"*", OperatorType::MULTIPLY},
    {"/", OperatorType::DIVIDE},
    {"%", OperatorType::MODULUS}
};

const std::regex var_init_pattern(
   "^\\s*\\[([^\\]]+)\\]\\s+([A-Za-z]\\w*)\\s+([A-Za-z_]\\w*)\\s*$"
);

const std::regex control_pattern("^#(\\w+)(?:\\s+(.*))?$");

const std::regex expression_pattern(
    R"(^\s*([A-Za-z_]\w*)\s*=\s*([A-Za-z_]\w*|\d+)\s*([+\-*/])\s*([A-Za-z_]\w*|\d+)\s*$)"
);

const std::regex only_ws("^\\s*$");

const std::regex sys_func_pattern(R"(^\s*@([A-Za-z_]\w*)\s*\(\s*([A-Za-z_]\w*)(?:\s*,\s*([A-Za-z_]\w*))?\s*\)\s*$)");

Command handle_var_init(const std::smatch& matches, int line_number);
Command handle_ctrl_flow(const std::smatch& matches, int line_number);
Command handle_expression(const std::smatch& matches, int line_number);
Command handle_sysfunc(const std::smatch& matches, int line_number);

void parse(const std::string& filename, std::vector<Command>* commands);