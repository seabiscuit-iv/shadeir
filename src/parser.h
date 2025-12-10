#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <regex>

#pragma once
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
    std::string value;
};

struct ControlFlowCommand {
    FlowCmdType flowCmdType;
    std::string conditional;
};

struct Expression {
    OperatorType op;
    std::string a;
    std::string b;
    std::string c;
};

struct SystemFunction {
    std::string function;
    std::string input;
};

struct Command {
    CommandType cmd_type;
    VariableInitializer var_init;
    ControlFlowCommand ctrl_flow;
    Expression expression;
    SystemFunction sys_func;
};

void parse(const std::string& filename, std::vector<Command>* commands);