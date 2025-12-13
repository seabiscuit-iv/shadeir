#include <iostream>
#include <fmt/format.h>
#include "parser.h"
#include "variable.h"
#include "base_ptx.h"
#include "logexpression.h"
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./app <filename>\n";
        return 1;
    }

    std::vector<Command> commands;
    parse(argv[1], &commands);

    std::vector<Variable> vars = get_variable_list(commands);

    // quick verifier
    for (Command &cmd : commands) {
        std::string cmd_type;
        
        if ( cmd.cmd_type == CommandType::VAR_INIT ) {
            cmd_type = "VAR_INIT";
        }
        else if ( cmd.cmd_type == CommandType::CTRL_FLOW ) {
            cmd_type = "CTRL_FLOW";
        }
        else if ( cmd.cmd_type == CommandType::EXPRESSION ) {
            cmd_type = "EXPRESSION";
        }
        else if ( cmd.cmd_type == CommandType::SYS_FUNC ) {
            cmd_type = "SYS_FUNC";
        }
        else {
            cmd_type = "ERROR";
        }

        fmt::println("{}", cmd_type);
    }



    // file writing
    std::ofstream out_file("shade-ir-out.ptx");
    out_file << base_ptx_start;

    for ( const Variable& var : vars ) {
        std::string type = var.data_type == VarDataType::FLOAT ? ".f32" : ".s32";

        out_file << ".reg " << type << " %r_" << var.name << ";" << std::endl;
    }

    out_file << "\n\n";

    for( Command &cmd : commands ) {
        if (cmd.cmd_type == CommandType::VAR_INIT) {
            continue;
        }
        else if (cmd.cmd_type == CommandType::CTRL_FLOW && cmd.ctrl_flow.flowCmdType == FlowCmdType::EXEC_MASK) {
            LogExpression exp = str_to_expression(cmd.ctrl_flow.left, cmd.ctrl_flow.comp, cmd.ctrl_flow.right, vars);
            std::string comp = comparator_to_ptx(exp.comp);

            std::string left_name = exp.left_is_literal ? fmt::format("{}", exp.l_left.l_int) : fmt::format("%r_{}", exp.left.name);
            std::string right_name = exp.right_is_literal ? fmt::format("{}", exp.l_right.l_int) : fmt::format("%r_{}", exp.right.name);

            out_file << "setp." << comp << ".s32   %exec_mask, " << left_name << ", " << right_name << ";" << std::endl;
        }
        else if (cmd.cmd_type == CommandType::EXPRESSION) {
            std::string a;
            if (is_number(cmd.expression.a)) {
                a = fmt::format("{}", cmd.expression.a);
            }
            else {
                a = fmt::format("%r_{}", find_var(cmd.expression.a, vars)->name);
            }

            std::string b;
            if (is_number(cmd.expression.b)) {
                b = fmt::format("{}", cmd.expression.b);
            }
            else {
                b = fmt::format("%r_{}", find_var(cmd.expression.b, vars)->name);
            }

            std::string c;
            if (is_number(cmd.expression.c)) {
                c = fmt::format("{}", cmd.expression.c);
            }
            else {
                c = fmt::format("%r_{}", find_var(cmd.expression.c, vars)->name);
            }

            std::string op = operator_to_ptx.at(cmd.expression.op);

            out_file << op << ".s32   " << c << ", " << a << ", " << b << ";" << std::endl;
        }
    }

    out_file << base_ptx_end;

    out_file.close();
    return 0;
}