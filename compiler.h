#pragma once
#include <vector>
#include <map>
#include <memory>
#include <cstdint>
#include <iostream>
#include "Parese.h"   
#include "VM.h"

class Compiler {
private:
    vector<ByteCodeInstruction> bc;
    vector<int32_t> consts;
    map<string, int32_t> vars;   
    int32_t nvi;

    int32_t add_c(int32_t v)
    {
        consts.push_back(v);
        return consts.size() - 1;
    }

    int32_t add_v(const string& n)
    {
        if (vars.find(n) == vars.end())
        {
            vars[n] = nvi++;
        }
        return vars[n];
    }

    void put(OpCode op, int32_t operand = 0)
    {
        bc.push_back(ByteCodeInstruction(op, operand));
    }

    void compile_node(std::shared_ptr<ASTNode> node)  
    {
        if (!node) return;

        if (auto numNode = std::dynamic_pointer_cast<NumberNode>(node))
        {                                                   
            int32_t v = std::stoi(numNode->value);         
            put(OpCode::LOAD_CONST, add_c(v));
        }
        else if (auto idNode = dynamic_pointer_cast<IdentifierNode>(node))
        {
            put(OpCode::LOAD_VAR, add_v(idNode->name));      
        }
        else if (auto binNode = dynamic_pointer_cast<BinaryOpNode>(node))
        {
            compile_node(binNode->left);                    
            compile_node(binNode->right);                    

            if (binNode->op == "+") put(OpCode::ADD);        
            else if (binNode->op == "-") put(OpCode::SUB);
            else if (binNode->op == "*") put(OpCode::MUL);
            else if (binNode->op == "/") put(OpCode::DIV);
        }
        else if (auto unaryNode = std::dynamic_pointer_cast<UnaryOpNode>(node))
        {
            compile_node(unaryNode->operand);
            if (unaryNode->op == "-") put(OpCode::NEG);
        }
        else if (auto exprStmt = std::dynamic_pointer_cast<ExpressionStatementNode>(node))
        {
            compile_node(exprStmt->expression);
            put(OpCode::PRINT);
        }
    }

public:
    Compiler() : nvi(0) {}

    void compile(std::shared_ptr<ProgramNode> program)
    {
        for (auto& stmt : program->statements)
        {
            compile_node(stmt);
        }
        put(OpCode::RETURN);
    }

    const std::vector<ByteCodeInstruction>& getBytecode() const
    {
        return bc;
    }

    const std::vector<int32_t>& getConstants() const
    {
        return consts;
    }

    void printBytecode() const
    {
        std::cout << "BYTECODE\n";
        std::cout << "Constants: ";
        for (int32_t c : consts) std::cout << c << " ";
        std::cout << "\n\nInstructions:\n";

        for (size_t i = 0; i < bc.size(); i++)
        {
            std::cout << "[" << i << "] ";
            switch (bc[i].opcode)
            {
                case OpCode::LOAD_CONST:
                    std::cout << "LOAD_CONST " << bc[i].operand; break;
                case OpCode::LOAD_VAR:
                    std::cout << "LOAD_VAR "   << bc[i].operand; break;
                case OpCode::STORE_VAR:
                    std::cout << "STORE_VAR "  << bc[i].operand; break;
                case OpCode::ADD:    std::cout << "ADD";    break;
                case OpCode::SUB:    std::cout << "SUB";    break;
                case OpCode::MUL:    std::cout << "MUL";    break;
                case OpCode::DIV:    std::cout << "DIV";    break;
                case OpCode::NEG:    std::cout << "NEG";    break;
                case OpCode::PRINT:  std::cout << "PRINT";  break;
                case OpCode::POP:    std::cout << "POP";    break;
                case OpCode::RETURN: std::cout << "RETURN"; break;
            }
            std::cout << "\n";
        }
    }
};
